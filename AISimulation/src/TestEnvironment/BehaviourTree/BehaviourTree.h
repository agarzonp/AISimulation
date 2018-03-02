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
		std::unique_ptr<BTNode> entityBehaviour= std::make_unique<BTNodeSelector>();

			// Work behaviour (sequence)
			//
			std::unique_ptr<BTNode> workBehaviour = std::make_unique<BTNodeSequence>();
				std::unique_ptr<BTNode> isTimeToWork = std::make_unique<BTNodeBlackboardCondition>("isTimeToWork", BTBlackboardOperator::IS_EQUAL, true);
				std::unique_ptr<BTNode> workSelector = std::make_unique<BTNodeSelector>();
					std::unique_ptr<BTNode> isWorking = std::make_unique<BTNodeBlackboardCondition>("isWorking", BTBlackboardOperator::IS_EQUAL, true);
					std::unique_ptr<BTNode> workAction = std::make_unique<BTNodeActionWork>();
				static_cast<BTNodeComposite*>(workSelector.get())->AddChild(isWorking);
				static_cast<BTNodeComposite*>(workSelector.get())->AddChild(workAction);
			static_cast<BTNodeComposite*>(workBehaviour.get())->AddChild(isTimeToWork);
			static_cast<BTNodeComposite*>(workBehaviour.get())->AddChild(workSelector);
			//
			// Work behaviour end

			// Sleep behaviour (sequence)
			//
			std::unique_ptr<BTNode> sleepBehaviour = std::make_unique<BTNodeSequence>();
				std::unique_ptr<BTNode> isTimeToSleep = std::make_unique<BTNodeBlackboardCondition>("isTimeToSleep", BTBlackboardOperator::IS_EQUAL, true);
				std::unique_ptr<BTNode> sleepSelector = std::make_unique<BTNodeSelector>();
					std::unique_ptr<BTNode> isSleeping = std::make_unique<BTNodeBlackboardCondition>("isSleeping", BTBlackboardOperator::IS_EQUAL, true);
					std::unique_ptr<BTNode> sleepSequence = std::make_unique<BTNodeSequence>();
						std::unique_ptr<BTNode> tryToSleep = std::make_unique<BTNodeDecoratorFilter>(BTNodeBlackboardCondition("isTimeToSleep", BTBlackboardOperator::IS_EQUAL, true));
							std::unique_ptr<BTNode> goToSleep = std::make_unique<BTNodeSequence>();
								std::unique_ptr<BTNode> goToDoor = std::make_unique<BTNodeActionGoToDoor>();
								std::unique_ptr<BTNode> checkDoorOpen = std::make_unique<BTNodeSelector>();
									std::unique_ptr<BTNode> isDoorOpen = std::make_unique<BTNodeBlackboardCondition>("isDoorOpen", BTBlackboardOperator::IS_EQUAL, true);
									std::unique_ptr<BTNode> requestOpenDoor = std::make_unique<BTNodeActionRequestOpenDoor>();
									static_cast<BTNodeComposite*>(checkDoorOpen.get())->AddChild(isDoorOpen);
									static_cast<BTNodeComposite*>(checkDoorOpen.get())->AddChild(requestOpenDoor);
								std::unique_ptr<BTNode> enterRoom = std::make_unique<BTNodeActionEnterRoom>();
								static_cast<BTNodeComposite*>(goToSleep.get())->AddChild(goToDoor);
								static_cast<BTNodeComposite*>(goToSleep.get())->AddChild(checkDoorOpen);
								static_cast<BTNodeComposite*>(goToSleep.get())->AddChild(enterRoom);
						static_cast<BTNodeDecorator*>(tryToSleep.get())->SetChild(goToSleep);
						std::unique_ptr<BTNode> sleepAction = std::make_unique<BTNodeActionSleep>();
					static_cast<BTNodeComposite*>(sleepSequence.get())->AddChild(tryToSleep);
					static_cast<BTNodeComposite*>(sleepSequence.get())->AddChild(sleepAction);
				static_cast<BTNodeComposite*>(sleepSelector.get())->AddChild(isSleeping);
				static_cast<BTNodeComposite*>(sleepSelector.get())->AddChild(sleepSequence);
			static_cast<BTNodeComposite*>(sleepBehaviour.get())->AddChild(isTimeToSleep);
			static_cast<BTNodeComposite*>(sleepBehaviour.get())->AddChild(sleepSelector);
			//
			// Sleep behaviour end

		static_cast<BTNodeComposite*>(entityBehaviour.get())->AddChild(workBehaviour);
		static_cast<BTNodeComposite*>(entityBehaviour.get())->AddChild(sleepBehaviour);
		//
		// Entity Behaviour end
		
		root = std::move(entityBehaviour);
	}
};

#endif // !BEHAVIOUR_TREE_H

