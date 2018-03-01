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

	void Update(BTBlackboard& blackBoard)
	{
		root->Run(blackBoard);
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
				std::unique_ptr<BTNode> workAction = std::make_unique<BTNodeActionWork>();
				static_cast<BTNodeComposite*>(workBehaviour.get())->AddChild(isTimeToWork);
				static_cast<BTNodeComposite*>(workBehaviour.get())->AddChild(workAction);
			//
			// Work behaviour end

			// Sleep behaviour (filter - sequence)
			//
			std::unique_ptr<BTNode> sleepBehaviour = std::make_unique<BTNodeDecoratorFilter>(BTNodeBlackboardCondition("isTimeToSleep", BTBlackboardOperator::IS_EQUAL, true));
				std::unique_ptr<BTNode> goToSleep = std::make_unique<BTNodeSequence>();
					std::unique_ptr<BTNode> goToDoor = std::make_unique<BTNodeActionGoToDoor>();
					std::unique_ptr<BTNode> checkDoorOpen = std::make_unique<BTNodeSelector>();
						std::unique_ptr<BTNode> isDoorOpen = std::make_unique<BTNodeBlackboardCondition>("isDoorOpen", BTBlackboardOperator::IS_EQUAL, true);
						std::unique_ptr<BTNode> requestOpenDoor = std::make_unique<BTNodeSequence>();
							std::unique_ptr<BTNode> requestOpenDoorAction = std::make_unique<BTNodeActionRequestOpenDoor>();
							std::unique_ptr<BTNode> waitUntilDoorOpen = std::make_unique<BTNodeActionWait>(std::chrono::seconds(5));
							static_cast<BTNodeComposite*>(requestOpenDoor.get())->AddChild(requestOpenDoorAction);
							static_cast<BTNodeComposite*>(requestOpenDoor.get())->AddChild(waitUntilDoorOpen);
						static_cast<BTNodeComposite*>(checkDoorOpen.get())->AddChild(isDoorOpen);
						static_cast<BTNodeComposite*>(checkDoorOpen.get())->AddChild(requestOpenDoor);
					std::unique_ptr<BTNode> enterRoom = std::make_unique<BTNodeActionEnterRoom>();
					static_cast<BTNodeComposite*>(goToSleep.get())->AddChild(goToDoor);
					static_cast<BTNodeComposite*>(goToSleep.get())->AddChild(checkDoorOpen);
					static_cast<BTNodeComposite*>(goToSleep.get())->AddChild(enterRoom);
			static_cast<BTNodeDecorator*>(sleepBehaviour.get())->SetChild(goToSleep);
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

