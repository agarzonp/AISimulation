# AI Simulation

Core systems:

- A Behaviour Tree system. This BT system has the ability to keep a persist evaluation during several frames, so the decision can be postponed until a request is satisfied(for example a pathfinding request). It is done by traversing the tree from the root, but redirecting the search to the running node.
This BT system has the possibility to abort current evaluation on the tree, so for example, no time will be wasted on an ongoing pathfinding request that is not needed anymore.

- A pathfinding system. This pathfinding system is using a grid as a search space and with two search algorithms(A* and JPS). The pathfinding system is able to queue pathing requests, process them among several frames and notify the result to the requester. It is optimised to allow to search for a certain amount of search revolutions for each frame, so in case that more iterations are needed, the request will be suspended and resumed in the next frame.

- A path following algorithm that uses a seek steering behaviour for entity movement.
