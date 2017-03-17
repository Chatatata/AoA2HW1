# AoA2HW1
Analysis of Algorithms II, Assignment I.

### Introduction

Breadth-first search and depth-first search are two algorithms used to find a value in a tree. These algorithms are similar to each other, the only difference between them is prioritization of element access. In order to solve the puzzle, we used those algorithms since the movement actions in the puzzle could be easily represented by a tree. However, in that approach, we are not going to find a single node, instead we want to find a node which makes the game detachable (finished). There are many ways to finish the game, and those algorithms try to find one of those ways with different approaches.

### Pseudo-code

#### Breadth-First Search

To make breadth-first search based solution of the state machine, given pseudo-code is used.
```
performBFS(battleMap):

  possible instructions = getPossibleInstructionsFor(battleMap: battleMap)

  machines = []

  for each\_possible\_instruction in &#39;possible instructions&#39;:

    new machine = GameStateMachine(map: battleMap)

    &#39;new machine&#39;.runInstruction(each\_possible\_instruction)

    if &#39;new machine&#39; is already calculated:

      continue

    else if &#39;new machine&#39; is detachable:

      return &#39;new machine&#39;

    else:

      add &#39;new machine&#39; to &#39;calculated machines&#39;

      add &#39;new machine&#39; to &#39;machines&#39;

  for each\_machine in &#39;machines&#39;:

    try:

      return performBFS(each\_machine)

    catch expansion not available error:

      continue

  throw expansion not available error
```

This function fetches the possible instructions, who are runnable in the given state machine, at first. Afterwards, it tries each possible instruction performing them in given state machine, and checks if it  solves the puzzle. However, there are caveats in that motivation: A state machine could have been analyzed, which had been, in terms, performed breadth-first search in the past.

If no solution was found, this function propagates an exception throw, as the worst case.

As a performance penalty, this function causes to copy the whole stack of exception stack traceback. However this could be easily avoided making inlining the function call in order to prevent spilling of the variables.

#### Depth-First Search

To make depth-first search based solution of the state machine, given pseudo-code is used.

```
performDFS(battleMap):

  possible instructions = getPossibleInstructionsFor(battleMap: battleMap)

  for each\_possible\_instruction in &#39;possible instructions&#39;:

    new machine = GameStateMachine(map: battleMap)

    &#39;new machine&#39;.runInstruction(each\_possible\_instruction)

    if &#39;new machine&#39; is already calculated:

      continue

    else if &#39;new machine&#39; is detachable:

      return &#39;new machine&#39;

    else:

      add &#39;new machine&#39; to &#39;calculated machines&#39;

      try:

        return performDFS(each\_machine)

      catch expansion not available error:

        continue

  throw expansion not available error
```

Likewise breadth-first search, this approach also possible instructions at first. Afterwards, for each available instruction, it performs the instruction in given state machine. If the result machine is detachable it returns the final state machine, if it is not, it performs depth-first search on that node.

This function also propagates an exception in case of failure, however unlike breadth-first search, this would not create performance penalty since the function recursion could be flattened by the compiler.

### Documentation

**Struct `Point`**: a POCO data structure representing axis and ordinate in two-dimensional coordinate system

**Struct `Brick`**: a POCO data structure representing bricks on the puzzle

**Class `MovementInstruction`**: a POCO data structure representing machine instructions recognizable by state machine

**State Machine**: This class is responsible of controlling the puzzle board. It is instantiated with an brick array, it takes instructions and performs them by dequeueing them in internals. It also interfaces the performed instructions and undone instructions. Therefore, a traceback could be analyzed by checking those functions. State machine halts when invalid (undoable) instruction is given to be performed. After halt, it is not able to run instructions again. A state-machine is _as per_ for step-by-step debugger.

**State Machine Controller**: This class is responsible of controlling the state machine by supplying valid instructions in order to get a detachability on it. When a state machine is detachable, it means it is solved by the current state. A detachable state machine does not consist finished problem, rather it prefers to choose first one-eye visible ahead solution of that machine.

**File Parser**: This class parses the file and returns a linear array of bricks in order to instantiate state machine.

### Analysis
#### Breadth-First Search

In breadth-first search, level-prioritized approach is performed instead of depth-prioritization.

```
performBFS(battleMap):

  possible instructions = getPossibleInstructionsFor(battleMap: battleMap)

  machines = []

  for each\_possible\_instruction in &#39;possible instructions&#39;:

    new machine = GameStateMachine(map: battleMap)

    &#39;new machine&#39;.runInstruction(each\_possible\_instruction)

    if &#39;new machine&#39; is already calculated:

      continue

    else if &#39;new machine&#39; is detachable:

      return &#39;new machine&#39;

    else:

      add &#39;new machine&#39; to &#39;calculated machines&#39;

      add &#39;new machine&#39; to &#39;machines&#39;

  for each\_machine in &#39;machines&#39;:

    try:

      return performBFS(each\_machine)

    catch expansion not available error:

      continue

  throw expansion not available error
```

For each vertex _V_ (possible instruction), it performs a detachability check for the state machine. Detachability check results in _O(B)_ complexity since each brick is checked against remaining bricks in current state, where _B_ is number of bricks. Therefore this will result in _O(V) = O(B)_. Since worst case complexity of breadth-first search algorithm is *O(|V| + |E|)*, and maximum number of edges could be _Emax = B \* 2_, the total complexity will be _O(|B|)_.

A cycle check also would cost as a linear lookup on state machine history, which has a complexity of _Θ(n)_.

#### Depth-First Search

In depth-first search, depth-prioritization is the main approach of the algorithm.

```
performDFS(battleMap):

  possible instructions = getPossibleInstructionsFor(battleMap: battleMap)

  for each\_possible\_instruction in &#39;possible instructions&#39;:

    new machine = GameStateMachine(map: battleMap)

    &#39;new machine&#39;.runInstruction(each\_possible\_instruction)

    if &#39;new machine&#39; is already calculated:

      continue

    else if &#39;new machine&#39; is detachable:

      return &#39;new machine&#39;

    else:

      add &#39;new machine&#39; to &#39;calculated machines&#39;

      try:

        return performDFS(each\_machine)

      catch expansion not available error:

        continue

  throw expansion not available error
```

Since the main motivation of the algorithm does not produce a difference from breadth-first search, the total complexity will be _O(|B|)_ again.

Likewise breadth-first search, a cycle check also would cost as a linear lookup on state machine history, which has a complexity of _Θ(n)_. In order perform cycle check, the old state machines should be inserted to a linear container. In terms of _std::vector_ template, this is performed with _O(1)_ complexity.

### Running Times

See [Running Times](https://raw.githubusercontent.com/Chatatata/AoA2HW1/master/analysis.pdf)

### Conclusion

Both algorithms work with same complexity, however the number of times each operation is performed changes implicitly. The main problem of the depth-first search is it tries to solve the puzzle from first available movement, therefore, depending on the input, this might be close to the worst case scenario. However, since the game is played with a finite state machine, both algorithms might be used in order to solve the puzzle. That way, especially depth-first search might help to improve the overall performance.

As a side note, the performance of both algorithms could be analyzed by utilizing Dijkstra&#39;s algorithm in order to find the shortest path in the tree. This would result in generating whole tree, which consists the state flowchart of the finite state machine, which makes it unusable in solving puzzle but rather making a performance analysis.

### License

MIT
