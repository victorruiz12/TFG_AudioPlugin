

namespace hdl{

	//Feedback Delay inherits from feedforward delay
	class FBDelay : FFDelay {
		//new constructor
		FBDelay(int maxSize = int(sampleRate * 5)){
			feedback = .6;
			//call the other constructor
			super(maxSize);
		}


		//feedback delay:https://skytracks.io/blog/understanding-delay-part-iii-using-delay-like-an-instrument/


		void setFeedback(double f){ feedback = f; }

		//we have to overwrite the process method
		double process(double x){
			dIdx += retuneSpeed * (delay - dIdx); // now its closer to our delay
			rIdx = wIdx - dIdx;
			if(rIdx<0)
				rIdx += size(); //pops up in the other side of the array again
			double y = x + getSample() * feedback; //following block diagram, getsample is our delay signal
			data[wIdx] = y;
			//we have to increment
			inc();
			return y;
		}

		//we add a new variable
		double feedback;
	}

	class FFDelay {
		//default value max size of the buffer equals to sample rate (1sec) *5 of audio
		FFDelay(int maxSize = int(sampleRate * 5)){
			data.length = maxSize;
			//init idx para que no vaya outofbounds
			wIdx = 0;
			dIdx = rIdx = 0;
			retuneSpeed = 8. / sampleRate; //already find out that has to be small
			//dividimos para que sea igual para todos los sampleRates
		}


		int size(){ return data.length; }

		void setDelayInSeconds(double d){ delay= d * sampleRate; }

		//process method, for each sample we will write data 
		//into our buffer at our current idx

		double process(double x){
			data[wIdx]=x;
			//we need to find in which idx our delay is,
			//destination idx is actually following the signal
			//use retuneSpeed to mix the current value from destin.idx with our delay-dIdx
			dIdx += retuneSpeed * (delay - dIdx); // now its closer to our delay

			//logic: buffer usually goes in circles and dIdx doesnt follow the circular motion
			//delay describes how far is from wIdx and dIdx exactly the same but it follows the delay
			//instead of being the delay, at retuneSpeed


			//current idx - delay
			//rIdx = wIdx - delay;
			//now is
			rIdx = wIdx - dIdx;

			if(rIdx<0)
				rIdx += size(); //pops up in the other side of the array again
			double y = getSample();
			inc();
			return y;
		}

		protected double getSample(){ return data [int(rIdx)]; } //returns data in our idx, rounded to int
		//because double value can't be accessing an array. We could do interpolation here but sounds nice.


		//increment. We could use inheritance (protected void inc, and protected double getSample)
		protected void inc(){
			++wIdx;
			if(wIdx == size()) //if idx reaches size
				wIdx = 0;
		}


		array<double> data; //buffer

		//this actually just describing one repetition

		//index write to all buffer
		double delay, dIdx, rIdx, retuneSpeed;
		int wIdx;
	}
}
