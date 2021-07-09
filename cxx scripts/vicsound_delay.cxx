/** \file default.cxx
*   Default script, with no processing function.
*   Will simply do nothing (full audio and MIDI bypass) but show its description.
*/
string name="Delay";
string description="Feedback Delay (Comb Filter)";

#include "C:\Users\Victor Ruiz\Documents\Blue Cat Audio\Blue Cat's Plug'n Script\Scripts\library/hdl.hxx"

array<string> inputParametersNames = {"Delay", "Feedback", "Gain"};
array<string> inputParametersUnits = {"s","%"};
array<string> inputParametersEnums = {};
array<double> inputParameters(inputParametersNames.length);
array<int> inputParametersSteps = {};
array<double> inputParametersMin = {0, 0 ,0};
array<double> inputParametersMax = {2, 1,1}; //flanger with small max param

//feedforward
hdl::FBDelay delayL, delayR;
double delay = -1, feedback = -1;

void processBlock(BlockData& data){
	setParameters();
	//llamada para cada bloque de muestras (búfer) proporcionado por el host
	//everytime the block starts we get new values


	for(uint s = 0; s<data.samplesToProcess; ++s)
		data.samples[0][s] = delayL.process(data.samples[0][s]); //overwrite left channel
	
	if(audioOutputsCount != 1) //si tenemos mas de un canal, utilizamos el derecho -> stereo, no surround
		for(uint s = 0; s<data.samplesToProcess; ++s)
			data.samples[1][s] = delayR.process(data.samples[1][s]);
}

void setParameters(){
	delay = inputParameters[0]; //input delay

	delayL.setDelayInSeconds(delay);
	delayR.setDelayInSeconds(delay);

	feedback = inputParameters[1]; //input feedback

	delayL.setFeedback(feedback);
	delayR.setFeedback(feedback);
}

double rint(double d){
	if(d < 0.) return floor(d - .5);
	else return floor(d + .5);
}