/** \file default.cxx
*   Default script, with no processing function.
*   Will simply do nothing (full audio and MIDI bypass) but show its description.
*/
string name="Simple Delay";
string description="Simple delay with no feedback";

#include "C:\Users\Victor Ruiz\Documents\Blue Cat Audio\Blue Cat's Plug'n Script\Scripts\library/hdl.hxx"

array<string> inputParametersNames = {"delay"};
array<string> inputParametersUnits = {};
array<string> inputParametersEnums = {};
array<double> inputParameters(inputParametersNames.length);
array<int> inputParametersSteps = {};
array<double> inputParametersMin = {0};
array<double> inputParametersMax = {4.9};

//feedforward
hdl::FFDelay delay;

void processBlock(BlockData& data){
	delay.setDelayInSeconds(inputParameters[0]);

	for(uint s = 0; s<data.samplesToProcess; ++s){
		//for monophonic, mid value
		double mid = 0;
		for(uint ch = 0; ch<audioOutputsCount; ++ch)
			mid += data.samples[ch][s];	//get samples in it
		mid /= audioOutputsCount;//divide through the mono outputs so it doesnt get too loud

		//delay processes mid and then overwrites
		mid = delay.process(mid);

		//we put that into our samples. Monophonic version of our sound plus the delay
		for(uint ch = 0; ch<audioOutputsCount; ++ch)
			data.samples[ch][s] +=mid;
		
	}
}

void updateInputParametersForBlock(const TransportInfo@ info){
}

double rint(double d){
	if(d < 0.) return floor(d - .5);
	else return floor(d + .5);
}