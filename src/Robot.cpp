#include "WPILib.h"

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	LiveWindow *lw;
	Encoder EncDist;
	Gyro gyro;
	int autoLoopCounter;
	float xvalue;
	float yvalue,motorspeed;
	float cir;
	int Auto,ang;
	int Rev,d1,d2,d3;
	float EncVal1,EncVal2,EncVal3;


public:
	void RobotInit() override {
		//CameraServer::GetInstance()->SetQuality(50);
		//std::shared_ptr<USBCamera> camera(new USBCamera("cam0", false));
		//camera->SetExposureManual(100); // change this value
		//camera->SetBrightness(1); // change this value
		//CameraServer::GetInstance()->StartAutomaticCapture(camera);

		//Cam.StartCapture();

	    CameraServer::GetInstance()->SetQuality(100);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}
	Robot() :
		myRobot(1, 0),	// these must be initialized in the same order
		stick(5),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		EncDist(0),
		gyro(1),
		autoLoopCounter(0),
		xvalue(0),
		yvalue(0),
		motorspeed(0),
		cir(25.13274123),
		Auto(0),
		ang(90),
		Rev(360),
		d1(12),
		d2(12),
		d3(12),
		EncVal1(0),
		EncVal2(0),
		EncVal3(0)
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
		EncVal2=(d2/cir)*Rev;
		EncVal3=(d3/cir)*Rev;

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
		if((Auto==1)||(Auto==2))
			{
					while(gyro.GetAngle()<ang)
					{
						abs(gyro.GetAngle()-ang)*(1/90)=motorspeed;
						myRobot.TankDrive(motorspeed/2,motorspeed/-2);
				}
			}
		while(EncDist.GetDistance()<EncVal1)
		{
			myRobot.Drive(-0.5, 0.0);
		}

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		xvalue = -stick.GetZ()*.5;
		yvalue = -stick.GetY();
		myRobot.ArcadeDrive(yvalue,xvalue);
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
