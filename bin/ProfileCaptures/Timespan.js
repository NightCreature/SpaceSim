class Timespan
{
	constructor(timeStamp, resolution)
	{
		this.timeStamp = timeStamp;
		this.resolution = resolution;
	}
	
	GetTimeString()
	{
		let dateString = "";
		
		let time = Math.abs(this.timeStamp / this.resolution);
		if (time > 1)
		{
			this.seconds = Math.trunc(time);
			dateString +=  time + " seconds ";
			time -= Math.trunc(time);
		}
		else
		{
			time *= 1000; //Ms
			if (time > 1)
			{
				this.miliseconds = Math.trunc(time); 
				dateString += time + " ms ";
				time -= Math.trunc(time);
			}
			else
			{
				time *= 1000; //micro seconds
				if (time > 1)
				{
					this.microseconds = Math.trunc(time);
					dateString += time + " us ";
					time -= Math.trunc(time);
				}
				else
				{
					this.nanoseconds = time * 1000;//ns
					dateString += time + " ns";
				}
			}
		}
		return dateString;
	}
}