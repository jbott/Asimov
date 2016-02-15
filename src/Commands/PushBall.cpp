#include "PushBall.h"

/**
 * Commands namespace with implementation
 * of PushBall command group, and nested
 * Push and Stopper command classes.
 */
namespace commands
{

// PushBall constructor:
PushBall::PushBall(Intake *intake) : CommandGroup("PushBall")
{
	intake_ = intake;
	push_ = new Push(intake);
	stop_ = new Stopper(intake);
	timeout_ = 0.0;

	SetInterruptible(false);

	this->AddSequential(push_);
	this->AddSequential(new WaitCommand(timeout_));
	this->AddSequential(stop_);
}

// PushBall main functions:
void PushBall::AddedTime(double timeout)
{
	timeout_ = timeout;
}

double PushBall::GetAddedTime() const
{
	return timeout_;
}

// Push constructor:
PushBall::Push::Push(Intake *intake)
{
	intake_ = intake;
	SetInterruptible(false);
}

// Push main functions
void PushBall::Push::Initialize()
{
	std::cout << "Intake : PushBall : Push : Started" << std::endl;
	if (intake_->GetState() == State_t::HOLDING)
	{
		intake_->SetState(State_t::PUSHING);
		intake_->OutakeBall();
	}
	else
	{
		std::cout << "ERROR: Invalid starting state (should be \"HOLDING\")";
	}
}

bool PushBall::Push::IsFinished()
{
	return !intake_->CheckSwitch();
}

// Stopper constructor:
PushBall::Stopper::Stopper(Intake *intake)
{
	std::cout << "Intake : PushBall : Stopper : Started" << std::endl;
	intake_ = intake;
	SetInterruptible(false);
}

// Stopper main functions:
bool PushBall::Stopper::IsFinished()
{
	return true;
}

void PushBall::Stopper::End()
{
	std::cout << "Intake : PushBall : Stopper : Ended" << std::endl;
	intake_->Stop();
	intake_->SetState(State_t::OFF);
}

} // end namespace commands
