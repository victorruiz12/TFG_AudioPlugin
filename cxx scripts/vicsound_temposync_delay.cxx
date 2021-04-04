/** \file default.cxx
*   Default script, with no processing function.
*   Will simply do nothing (full audio and MIDI bypass) but show its description.
*/
string name="Temposync Delay";
string description="Temposync Feedback Delay (Conv filter)";

#include "C:\Users\Victor Ruiz\Documents\Blue Cat Audio\Blue Cat's Plug'n Script\Scripts\library/hdl_temposync.hxx"


//retuneSpeed changes the speed fast in the low values, slowly in high
array<string> inputParametersNames = { "x", "y", "feedback", "retuneSpeed" };
array<string> inputParametersUnits = {};
array<string> inputParametersEnums = {};
array<double> inputParameters(inputParametersNames.length);
array<int> inputParametersSteps = {64,64,-1,-1};
array<double> inputParametersMin = { 1, 1, 0, .1};
array<double> inputParametersMax = { 64, 64, 1, 1.};

//feedforward
hdl::FBDelay delayL, delayR;
hdl::RateHandler rateHandler;
double feedback = -1, retune = -1;

void processBlock(BlockData& data){
	//in BlockData is the transport info
	setParameters(data.transport.get_bpm()); //data transport in the manual (search bpm)
	//everytime the block starts we get new values


	for(uint s = 0; s<data.samplesToProcess; ++s)
		data.samples[0][s] = delayL.process(data.samples[0][s]); //overwrite left channel
	
	if(audioOutputsCount != 1) //stereo, no surround
		for(uint s = 0; s<data.samplesToProcess; ++s)
			data.samples[1][s] = delayR.process(data.samples[1][s]);
}

void setParameters(double bpm){
	if(rateHandler.process(bpm, inputParameters[0], inputParameters[1])){
		//delay = inputParameters[1];

		delayL.setDelayInSamples(rateHandler.getRate());
		delayR.setDelayInSamples(rateHandler.getRate());	
	}

	feedback = inputParameters[2];

	delayL.setFeedback(feedback);
	delayR.setFeedback(feedback);

	if(retune != inputParameters[3]){
		retune = inputParameters[3];
		delayL.setRetuneInSeconds(retune);
		delayR.setRetuneInSeconds(retune);
	}
}

double rint(double d){
	if(d < 0.) return floor(d - .5);
	else return floor(d + .5);
}