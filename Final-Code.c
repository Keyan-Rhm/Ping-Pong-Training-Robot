/*Get Time Function: Allows player to set the time for the practice session */

float getTime()
{
	float gameTime = 0;

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 2)
		{
			while (nNxtButtonPressed == 2)
			{}
			gameTime -= 30;
			eraseDisplay();

		}

		else if(nNxtButtonPressed == 1)
		{
			while (nNxtButtonPressed == 1)
			{}
			gameTime += 30;
			eraseDisplay();
		}

		displayString(0, "Time (s): %d", gameTime);
		displayString(1, "Press middle ");
		displayString(2, "button if OK");
	}
	while (nNxtButtonPressed == 3)
	{}
	eraseDisplay();
	return gameTime * 1000; //for milliseconds
}

int getLevel()
{
	displayString(0, "Choose level:");
	displayString(1, "Left Button  =1");
	displayString(2, "Middle Button=2");
	displayString(3, "Right Button =3");

	while (nNxtButtonPressed == -1)
	{}
	int captureButton = nNxtButtonPressed;
	eraseDisplay();
	while (nNxtButtonPressed != -1)
	{}
	if(captureButton == 2)
		return 1;
	else if(captureButton == 3)
		return 2;
	else if(captureButton == 1)
		return 3;
	//else, return error = -1

	return -1;

}


//Setting the level
void setLevel(int & level, int & serveFrequency)
{
	if(level == 1)
		serveFrequency = -10;
	else if(level == 2)
		serveFrequency = -30;
	else if(level == 3)
		serveFrequency = -50;

	displayString(0,"Level is:%d", level);
	displayString(1,"Launch speed:%d", -serveFrequency);
	displayString(2,"Up for it, Frosh?");
	displayString(3, "Left=No");
	displayString(4, "Other=Yes");
	while (nNxtButtonPressed == -1)
	{}
	int captureButton = nNxtButtonPressed;
	while (nNxtButtonPressed != -1)
	{}
	eraseDisplay();
	if (captureButton == 2)
	{
		level = getLevel();
		setLevel(level, serveFrequency);
	}
}



/*Number of Hits Function - tallies the number of target hits the user makes*/
void numHits(int & tally, int endTime)
{
	while(SensorValue[S1] != 1 && SensorValue[S2] != 1 && time1[T1] < endTime)
	{}
	while((SensorValue[S1] == 1 || SensorValue[S2] == 1) && time1[T1] < endTime)
	{}
	if (time1[T1] < endTime)//prevents adding 1 extra hit after time has expired, before functions return to main
		tally++;
}

//Mario Flagpole Fanfare End Song
void endGameSong()
{
	int noteFreq[31] = {196, 262, 330, 392, 523, 659, 784,
		659, 208, 262, 311, 415, 523, 622, 831, 622, 233, 294,
		349, 466, 587, 698, 932, 0, 932, 0, 932, 0, 932, 0, 1046};

	int halfNoteTime = 50;
	int noteTime[31] = {halfNoteTime/3, halfNoteTime/3, halfNoteTime/3, halfNoteTime/3,
		halfNoteTime/3, halfNoteTime/3, halfNoteTime, halfNoteTime, halfNoteTime/3, halfNoteTime/3,
		halfNoteTime/3, halfNoteTime/3, halfNoteTime/3, halfNoteTime/3, halfNoteTime, halfNoteTime,
		halfNoteTime/3, halfNoteTime/3, halfNoteTime/3, halfNoteTime/3, halfNoteTime/3, halfNoteTime/3,
		halfNoteTime, halfNoteTime/6, halfNoteTime/6, halfNoteTime/6, halfNoteTime/6, halfNoteTime/6,
		halfNoteTime/6, halfNoteTime/6, halfNoteTime*2};

	for(int i = 0; i < 31; i++)
	{
		playTone(noteFreq[i], noteTime[i]);
		wait1Msec(200);
	}
}

//Feedback Function - to be outputted at end of program
void feedback (int serves, int hits, float minutes)
{
	float percentHits = (hits * 1.0)/serves * 100;
	displayString(0, "You survived!");
	displayString(1, "# serves: %d", serves);
	displayString(2, "On target: %d", hits);
	displayString(3, "Hits/Serve: %d%%", percentHits); //round percent down to whole percent
	displayString(4, "Serve/Min: %d", serves/minutes);
	displayString(5, "Hits/Min: %d", hits/minutes);
	displayString(6, "Hit 3 to end!");

	while (nNxtButtonPressed != 3)
	{}
}

//talk Function
void talk(int level)
{
	nVolume = 3;
	if(level == 1)
	{
		playSoundFile("Level1.rso");
		wait10Msec(400);
	}

	else if(level == 2)
	{
		playSoundFile("Level2.rso");
		wait10Msec(400);
	}

	else if(level == 3)
	{
		playSoundFile("Level3.rso");
		wait10Msec(400);
	}

	playSound(soundUpwardTones);
	wait10Msec(150);

	for(int i = 0; i < 8; i++)
	{
		playSound(soundBeepBeep);
		wait10Msec(100);
	}

	playSound(soundLowBuzzShort);
	wait10Msec(100);

}

//Adds spin to the ball
void spin(int level)
{
	if (level == 2)
		motor[motorA] = 80;

	else if(level == 3)
		motor[motorA] = 60;
}


task main()
{
	displayString(0,"Press button :D");
	while (nNxtButtonPressed == -1)
	{}
	while (nNxtButtonPressed != -1)
	{}

	eraseDisplay();

	//Variables needed
	nMotorEncoder[motorC] = 0;
	SensorType[S1] = sensorTouch;
	int level = 0;
	int frequency = 0;
	int hits = 0;
	float timeFinish = 0;
	int numServes = 0;

	//Getting the time
	timeFinish = getTime();

	//getting the level
	level = getLevel();

	setLevel(level, frequency);

	talk(level);

	//Starting and using the robot

	motor[motorA] = 100;
	wait10Msec(1500);
	/*motor A is weak, so we give it time to power up*/

	motor[motorB] = 100;
	wait10Msec(1500); //a bit of time to speed up all the motors

	motor[motorC] = frequency;

	time1[T1] = 0;

	while(time1[T1] <= timeFinish)
		numHits(hits, timeFinish);

	int saveMotorEncoder = nMotorEncoder[motorC]; 
	/*save motor encoder, since the motors will still spin until they turn off*/

	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	numServes = - saveMotorEncoder / 360; 
	//(motor was going backwards)
	wait1Msec(2000);
	endGameSong ();

	feedback(numServes, hits, timeFinish/60000.0);

}
