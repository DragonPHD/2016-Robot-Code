#include "WPILib.h"

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	LiveWindow *lw;
	Encoder EncDist,lEnc,rEnc;//measure rotational distance
	AnalogGyro gyro;//measure angle
	DigitalInput Autostop,captured,lSwitch;//switches
	DigitalInput rSwitch;
	//bool intup,intdown; Unused
	bool toggle,latch;
	VictorSP launcherl,launcherr,intake;//Motor Conroller PWM
	CanTalonSRX actintake,sm2kl,sm2kr;//Motor Controllers CAN
	int autoLoopCounter;
	float gyrorate;
	float xvalue;
	float yvalue;
	float motorspeed;
	float cir;
	int Auto,ang;
	int Rev,d1;
	float EncVal1;//storedenc; Unused


public:
	void RobotInit() override {
	    CameraServer::GetInstance()->SetQuality(100);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}
	Robot() :
		//These must be enabled in the order declared above
		myRobot(4, 5),	//Originally (1,0)
		stick(5),		//Needs to be in USB 5
		lw(LiveWindow::GetInstance()),
		EncDist(0,1), 	//Encoder mounted on DRive for Autonomous
		lEnc(2,3),		//Encoder for left side extension of climber
		rEnc(4,5),		//Encoder for Right side extension of climber
		gyro(1),		//Gyro for Autonomous shooting
		Autostop(6),	//Limit Switch for when we reach the batter
		captured(7),	//Limit Switch for when we have a ball in the robot
		lSwitch(8),		//Limit Switch for left side retraction of climber
		rSwitch(9),		//Limit Switch for right side retraction of climber
		//intup(0),		Unused
		//intdown(0),	Unused
		toggle(0),		//Toggle for Drive reverse
		latch(0),		//Latch for Drive Reverse
		launcherl(0),	//Left motor of Launcher
		launcherr(1),	//Right motor of Launcher
		intake(3),		//Motor to move the intake in/out
		//tester(1),	Test Motor
		actintake(1),	//Motor To actuate Intake Up/Down
		sm2kl(2),		//Climbing Motor for the Left Side
		sm2kr(3),		//Climbing Motor for the Right Side
		autoLoopCounter(0),//Variable from example code
		gyrorate(0),	//Test Gyro Value
		xvalue(0),		//Variable to isolate joystick X
		yvalue(0),		//Variable to isolate joystick Y
		motorspeed(0),	//Variable to isolate speed of motor for turning
		cir(25.13274123),//Circumference of wheels
		Auto(0),		//Variable for which defence we're crossing
		ang(90),		//Create 90deg constant for gyro
		Rev(360),		//Create 360deg constant for encoder
		d1(0),			//Variable for Auto driving distance
		EncVal1(0)		//Variable for how much the wheels need to turn
		//storedenc(0)	Unused
	{
		myRobot.SetExpiration(0.1); //Code from Example
	}

private:
	void AutonomousInit()	//Initialize Autonomous
	{
		gyro.Reset();		//Reset Gyro Value

		if(Auto==1)
		{
			d1=84;			//Distance to center from Defense 1
		}
		else if(Auto==2)
		{
			d1=36;			// ~~ Defense 2
		}
		else if(Auto==3)
		{
			d1=12;			// ~~ Defense 3
			ang=-ang;		//Tell Robot it will Turn Left to Center
		}
		else if(Auto==4)
		{
			d1=60;			// ~~ Defense 4
			ang=-ang;		// ~~
		}

		autoLoopCounter = 0;	//Variable to make Auto run once
		EncVal1=(d1/cir)*Rev;	//Calculate Encoder value need to go d1

	}

	void AutonomousPeriodic()	//Start Autonomous
	{
		while(autoLoopCounter==0)	//Has it run only once
		{
		if((Auto==1)||(Auto==2))	//Are we turning Right?
			{
				while(gyro.GetAngle()<ang)	//Go until we surpass 90deg
				{
					//Turn Distance from 90deg into value robot can read
					motorspeed=(1/90)*abs(gyro.GetAngle()-ang);
					//Make motors drive in a way to utilize value
					myRobot.TankDrive(motorspeed/-2,motorspeed/2);
				}
			}
		if((Auto==3)||(Auto==4))	//Are we turning Left?
		{
			while(gyro.GetAngle()>ang)	//Go until we surpass -90deg
			{
				//Turn distance from -90deg
				motorspeed=(1/90)*abs(gyro.GetAngle()-ang);
				//Make motors drive in a way to utilize new value
				myRobot.TankDrive(motorspeed/-2,motorspeed/2);
			}
		}
		//Reset Encoder to 0
		EncDist.Reset();
		//Have we gone the distance
		while(EncDist.GetDistance()<EncVal1)
		{
			//Drive forward
			myRobot.Drive(-0.5, 0.0);
		}
		//If we turned right, turn left now/right,turn left
		ang = -ang;
		//Same as above
		if((Auto==1)||(Auto==2))
		{
			while(gyro.GetAngle()<ang)
			{
				motorspeed=(1/90)*abs(gyro.GetAngle()-ang);
				myRobot.TankDrive(motorspeed/-2,motorspeed/2);
			}
		}
		if((Auto==3)||(Auto==4))
		{
			while(gyro.GetAngle()>ang)
			{
				motorspeed=(1/90)*abs(gyro.GetAngle()-ang);
				myRobot.TankDrive(motorspeed/-2,motorspeed/2);
			}
		}
		launcherl.Set(1.0);		//Start left launcher motor
		launcherr.Set(-1.0);	//Start Right Launcher Motor
		while(Autostop.Get())	//Until we hit the step
		{
			//Drive Forward
			myRobot.Drive(-0.5,0.0);
		}
		//Stop Driving
		myRobot.Drive(0.0,0.0);
		//Feed ball into Launcher
		intake.Set(1.0);
		autoLoopCounter=1;		//Stop Autonomous
		}
	}

	void TeleopInit()		//Initiate Teleoperated
	{
		//Set Variable to false and initialize items
		toggle=false;
		latch=false;
		SmartDashboard::init();
		gyro.InitGyro();
		gyro.Calibrate();
		//autoLoopCounter=rSwitch.Get();	Unused
	}

	void TeleopPeriodic()	//Teleoperated Begins
	{
		if(stick.GetRawButton(3))	//Push->Move Climber Down
		{
			sm2kl.Set(1.0);
			sm2kr.Set(1.0);
		}
		//Release or we move all the way down -> Stop climber
		if(!lSwitch.Get()||!rSwitch.Get()||!stick.GetRawButton(3))
		{
			sm2kr.Set(0.0);
			sm2kl.Set(0.0);
		}
		if(stick.GetRawButton(5))	//Push->Move Climber Up
		{
			sm2kl.Set(-1.0);
			sm2kr.Set(-1.0);
		}
		//Release or if we go too far-> Stop Climber
		if((lEnc.GetDistance() > 1080)||(rEnc.GetDistance() > 1080)
				||!stick.GetRawButton(5))
		{
			sm2kl.Set(0.0);
			sm2kr.Set(0.0);
		}
		//Push->Move intake up
		if(stick.GetRawButton(10))
		{
			actintake.Set(1.0);
		}
		//Push->Move intake down
		else if(stick.GetRawButton(9))
		{
			actintake.Set(-1.0);
		}
		//if neither are being pressed-> stop motor
		else
		{
			actintake.Set(0.0);
		}
		//Use twist to turn, inverse it, and half it
		xvalue = -stick.GetZ()*.5;
		//Use forward/backward to move and inverse it
		yvalue = -stick.GetY();
		//Toggle system for drive inversion
		if(stick.GetRawButton(7)&&!toggle)	//Button pressed and latch not active
		{
			//Activate Latch
			latch=!latch;
			//toggle=true;	Unused
		}
		/*		Unused
		if(!stick.GetRawButton(7)&&toggle)
		{
			toggle=false;
		}
		*/
		if(latch)	//Latch Active
		{
			//Make forward backward and backward forward
			yvalue=-yvalue;
		}
		//Push->Shoot ball
		if(stick.GetRawButton(1))
		{
			//Opposing motors
			launcherl.Set(1);
			launcherr.Set(-1);
		}
		//Push->Reverse shooter
		else if(stick.GetRawButton(2))
		{
			//Opposing motors
			launcherl.Set(-1);
			launcherr.Set(1);
		}
		//If neither happens
		else
		{
			//Stop motors
			launcherl.Set(0);
			launcherr.Set(0);
		}
		//Push->Pull in ball
		if(stick.GetRawButton(12))
		{
			//Pull ball
			intake.Set(1.0);
		}
		//Push->Push out ball
		else if(stick.GetRawButton(11))
		{
			//Push Ball
			intake.Set(-1);
		}
		//neither are pressed
		else
		{
			//Stop intake
			intake.Set(0);
		}
		//Testing values
		gyrorate= gyro.GetAngle();
		myRobot.ArcadeDrive(yvalue,xvalue);		//Robot Drive

		//Display values
		SmartDashboard::PutNumber("Gyro",gyrorate);
		SmartDashboard::PutBoolean("Gyro Port",gyro.CheckPWMChannel(2));
		SmartDashboard::PutNumber("Gyro Rate",gyro.GetRate());
		SmartDashboard::PutNumber("Gyro Angle",gyro.GetAngle());
		SmartDashboard::PutNumber("Enc",EncDist.GetDistance());
		SmartDashboard::PutNumber("Switch",autoLoopCounter);
		SmartDashboard::PutBoolean("latch",latch);
		SmartDashboard::PutNumber("Button",stick.GetRawButton(5));
		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
	}

	//Dunno what to do with this
	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
