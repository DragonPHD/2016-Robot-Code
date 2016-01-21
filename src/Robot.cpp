#include "WPILib.h"

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;
	float xvalue;
	float yvalue;

public:
	Robot() :
		myRobot(1, 0),	// these must be initialized in the same order
		stick(5),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		xvalue(0),
		yvalue(0)
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
		xvalue = -stick.GetX();
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
