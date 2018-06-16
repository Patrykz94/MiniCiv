#include "StateMachine.h"

void StateMachine::AddState(StateRef newStateIn, bool isReplacingIn)
{
	isAdding = true;
	isReplacing = isReplacingIn;

	newState = std::move(newStateIn);
}

void StateMachine::RemoveState()
{
	isRemoving = true;
}

void StateMachine::ProcessStateChanges()
{
	if (isRemoving && !states.empty())
	{
		states.pop();

		if (!states.empty())
		{
			states.top()->Resume();
		}

		isRemoving = false;
	}

	if (isAdding)
	{
		if (!states.empty())
		{
			if (isReplacing)
			{
				states.pop();
			}
			else
			{
				states.top()->Pause();
			}
		}

		states.push(std::move(newState));
		states.top()->Init();
		isAdding = false;
	}
}

StateRef & StateMachine::GetActiveState()
{
	return states.top();
}
