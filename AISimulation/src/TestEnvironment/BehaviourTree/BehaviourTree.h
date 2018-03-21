#ifndef BEHAVIOUR_TREE_H
#define BEHAVIOUR_TREE_H

#include <memory>

#include "Blackboard/BTBlackboard.h"
#include "Nodes/BTNodes.h"

class BehaviourTree
{
	// root
	std::unique_ptr<BTNode> root;

public:

	void Update(BTBlackboard& blackboard)
	{
		root->Run(blackboard);
	}

	void Build()
	{
		// Entity behaviour (selector)
		//
		auto entityBehaviour= std::make_unique<BTNodeSelector>();

			// Work behaviour (selector)
			//
			auto workBehaviour = std::make_unique<BTNodeSelector>();
				auto workSequence = std::make_unique<BTNodeSequence>();
					auto isTimeToWork = std::make_unique<BTNodeBlackboardCondition>("isTimeToWork", BTBlackboardOperator::IS_EQUAL, true);
					auto workSelector = std::make_unique<BTNodeSelector>();
						auto isWorking = std::make_unique<BTNodeBlackboardCondition>("isWorking", BTBlackboardOperator::IS_EQUAL, true);
						auto workAction = std::make_unique<BTNodeActionWork>();
					workSelector->AddChild(std::move(isWorking));
					workSelector->AddChild(std::move(workAction));
				workSequence->AddChild(std::move(isTimeToWork));
				workSequence->AddChild(std::move(workSelector));
				auto setNotWorking = std::make_unique<BTNodeDecoratorInverter>();
					auto setNotWorkingAction = std::make_unique<BTNodeActionSetBlackboard>("isWorking", false);
				setNotWorking->SetChild(std::move(setNotWorkingAction));
			workBehaviour->AddChild(std::move(workSequence));
			workBehaviour->AddChild(std::move(setNotWorking));
			//
			// Work behaviour end

			// Sleep behaviour (selector)
			//
			auto sleepBehaviour = std::make_unique<BTNodeSelector>();
				auto sleepBehaviourSequence = std::make_unique<BTNodeSequence>();
					auto isTimeToSleep = std::make_unique<BTNodeBlackboardCondition>("isTimeToSleep", BTBlackboardOperator::IS_EQUAL, true);
					auto sleepSelector = std::make_unique<BTNodeSelector>();
						auto isSleeping = std::make_unique<BTNodeBlackboardCondition>("isSleeping", BTBlackboardOperator::IS_EQUAL, true);
						auto sleepSequence = std::make_unique<BTNodeSequence>();
							auto tryToSleep = std::make_unique<BTNodeParallelMonitor>(BTNodeParallel::Policy::RequireAll, BTNodeParallel::Policy::RequireOne);
								// conditions
								auto condition = std::make_unique<BTNodeBlackboardCondition>("isTimeToSleep", BTBlackboardOperator::IS_EQUAL, true);
								tryToSleep->AddCondition(std::move(condition));
								// behaviours to monitor
								auto goToSleep = std::make_unique<BTNodeSequence>();
									auto goToDoor = std::make_unique<BTNodeActionGoToDoor>();
									auto checkDoorOpen = std::make_unique<BTNodeSelector>();
										auto isDoorOpen = std::make_unique<BTNodeBlackboardCondition>("isDoorOpen", BTBlackboardOperator::IS_EQUAL, true);
										auto requestOpenDoor = std::make_unique<BTNodeActionRequestOpenDoor>();
										checkDoorOpen->AddChild(std::move(isDoorOpen));
										checkDoorOpen->AddChild(std::move(requestOpenDoor));
									auto enterRoom = std::make_unique<BTNodeActionEnterRoom>();
									goToSleep->AddChild(std::move(goToDoor));
									goToSleep->AddChild(std::move(checkDoorOpen));
									goToSleep->AddChild(std::move(enterRoom));
								tryToSleep->AddBehaviour(std::move(goToSleep));
								//
							auto sleepAction = std::make_unique<BTNodeActionSleep>();
						sleepSequence->AddChild(std::move(tryToSleep));
						sleepSequence->AddChild(std::move(sleepAction));
					sleepSelector->AddChild(std::move(isSleeping));
					sleepSelector->AddChild(std::move(sleepSequence));
				sleepBehaviourSequence->AddChild(std::move(isTimeToSleep));
				sleepBehaviourSequence->AddChild(std::move(sleepSelector));
				auto setNotSleeping = std::make_unique<BTNodeDecoratorInverter>();
					auto setNotSleepingAction = std::make_unique<BTNodeActionSetBlackboard>("isSleeping", false);
				setNotSleeping->SetChild(std::move(setNotSleepingAction));
			sleepBehaviour->AddChild(std::move(sleepBehaviourSequence));
			sleepBehaviour->AddChild(std::move(setNotSleeping));
			//
			// Sleep behaviour end

		entityBehaviour->AddChild(std::move(workBehaviour));
		entityBehaviour->AddChild(std::move(sleepBehaviour));
		//
		// Entity Behaviour end
		
		root = std::move(entityBehaviour);
	}
};

#endif // !BEHAVIOUR_TREE_H

