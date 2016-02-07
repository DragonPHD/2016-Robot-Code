#include "WPILib.h"

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	LiveWindow *lw;
	Encoder EncDist,lEnc,rEnc;
	AnalogGyro gyro;
	DigitalInput Autostop,captured,loaded,lSwitch,rSwitch;
	bool intup,intdown;
	bool toggle,latch;
	VictorSP launcher,intake;
	CanTalonSRX actintake,sm2kl,sm2kr;
	int autoLoopCounter;
	float xvalue;
	float yvalue;
	float motorspeed;
	float cir;
	int Auto,ang;
	int Rev,d1;
	float EncVal1;


public:
	void RobotInit() override {
	    CameraServer::GetInstance()->SetQuality(100);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		captured = new DigitalInput(0);
	}
	Robot() :
		myRobot(1, 0),	// these must be initialized in the same order
		stick(5),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		EncDist(0,1),
		lEnc(0,3),
		rEnc(4,6),
		gyro(0),
		Autostop(1),
		captured(0),
		loaded(0),
		lSwitch(0),
		rSwitch(0),
		intup(0),
		intdown(0),
		toggle(0),
		latch(0),
		launcher(5),
		intake(0),
		actintake(18),
		sm2kl(3),
		sm2kr(5),
		autoLoopCounter(0),
		xvalue(0),
		yvalue(0),
		motorspeed(0),
		cir(25.13274123),
		Auto(0),
		ang(90),
		Rev(360),
		d1(12),
		EncVal1(0)
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void AutonomousInit()
	{
		gyro.Reset();

		if(Auto==1)
		{
			d1=84;
		}
		else if(Auto==2)
		{
			d1=36;
		}
		else if(Auto==3)
		{
			d1=12;
			ang=-ang;
		}
		else if(Auto==4)
		{
			d1=60;
			ang=-ang;
		}

		autoLoopCounter = 0;
		EncVal1=(d1/cir)*Rev;

	}

	void AutonomousPeriodic()
	{
		if((Auto==1)||(Auto==2))
			{
				while(gyro.GetAngle()<ang)
				{
					motorspeed=(1/90)*abs(gyro.GetAngle()-ang);
					myRobot.TankDrive(motorspeed/2,motorspeed/-2);
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
		while(EncDist.GetDistance()<EncVal1)
		{
			myRobot.Drive(-0.5, 0.0);
		}
		ang = -ang;
		if((Auto==1)||(Auto==2))
		{
			while(gyro.GetAngle()<ang)
			{
				motorspeed=(1/90)*abs(gyro.GetAngle()-ang);
				myRobot.TankDrive(motorspeed/2,motorspeed/-2);
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
		launcher.Set(1.0);
		while(!Autostop.Get())
		{
			myRobot.Drive(-0.5,0.0);
		}
		myRobot.Drive(0.0,0.0);
		intake.Set(1.0);

	}

	void TeleopInit()
	{
		toggle=false;
		latch=false;
		SmartDashboard::init();
		SmartDashboard::PutNumber("Tests",0);
	}

	void TeleopPeriodic()
	{
		if(stick.GetRawButton(3))
		{
			sm2kl.Set(1.0);
			sm2kr.Set(1.0);
		}
		if(lSwitch.Get()||rSwitch.Get())
		{
			sm2kr.Set(0.0);
			sm2kl.Set(0.0);
			SmartDashboard::PutNumber("Tests",675);
		}
		if(stick.GetRawButton(5))
		{
			sm2kl.Set(-1.0);
			sm2kr.Set(-1.0);
			SmartDashboard::PutNumber("Tests",675);
		}
		if((lEnc.GetDistance() > 1080)||(rEnc.GetDistance() > 1080))
		{
			sm2kl.Set(0.0);
			sm2kr.Set(0.0);
		}
		if(stick.GetRawButton(10))
		{
			actintake.Set(1.0);
		}
		else if(stick.GetRawButton(9))
		{
			actintake.Set(-1.0);
		}
		else
		{
			actintake.Set(0.0);
		}

		xvalue = -stick.GetZ()*.5;
		yvalue = -stick.GetY();
		if(stick.GetRawButton(7)&&!toggle)
		{
			latch=!latch;
			toggle=true;
		}
		if(!stick.GetRawButton(7)&&toggle)
		{
			toggle=false;
		}
		if(latch)
		{
			yvalue=-yvalue;
		}
		if(stick.GetRawButton(1))
		{
			launcher.Set(1.0);
		}
		if(stick.GetRawButton(11))
		{
			intake.Set(1.0);
		}
		myRobot.ArcadeDrive(yvalue,xvalue);
		SmartDashboard::PutNumber("Captured",captured.Get());
		SmartDashboard::PutBoolean("Loaded",loaded.Get());
		SmartDashboard::PutBoolean("latch",latch);
		SmartDashboard::PutNumber("Button",stick.GetRawButton(7));
		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};
/*
 * #include "WPILib.h"
#include "SmartJoystick.h"

class Robot: public IterativeRobot
{
	SmartJoystick* operatorstick = new SmartJoystick(1);
	RobotDrive myRobot; // robot drive system
	SmartJoystick stick; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;

	float y = operatorstick->GetY();

public:
	Robot() :
		myRobot(1, 0),	// these must be initialized in the same order
		stick(5),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0)
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void AutonomousInit()
	{
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{
		if(autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
			} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)

 */

START_ROBOT_CLASS(Robot)
