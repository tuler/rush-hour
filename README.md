# Rush Hour

Rush Hour is a sliding block puzzle game originally created by [Nob Yoshigahara](https://en.wikipedia.org/wiki/Nob_Yoshigahara) in the 1970s and published by ThinkFun.

The goal is to move cars to get the red car out of the parking lot.
The game is played on a 6x6 grid, where each car takes up 2 or 3 squares.
The cars can only move horizontally or vertically, and they can't move through each other.
The game has 200 levels of increasing difficulty, and the goal is to solve each level as fast as possible.

It was developed for the [RIVES Jam #3](https://itch.io/jam/rives-jam-3).

Controls:

Arrow keys: move the selected car.
Any other key: select the next movable car.

## Building

After [installing RIV SDK](https://rives.io/docs/riv/developing-cartridges#installing-the-riv-sdk), you can build the game by running the following command:

```shell
make
```

## Running

```shell
make run
```

## Configuration

The difficulty of the game can be adjusted by passing the following argument:

```
--time-per-move 3000
```

This will give the player 3 seconds times the minimum number of moves to solve the level. For instance, if a level can be solved in 4 moves, the player will have 12 seconds to solve it.

The levels are loaded from a file (or from the incard). The file has one puzzle per line, with the following example format:

```
03 ooCoooooCoooAACoooBBBooooooooooooooo 23
```

This defines a puzzle that can be solved in 3 moves, with the following board configuration. The last number can be ignored.

```
o o C o o o
o o C o o o
A A C o o o
B B B o o o
o o o o o o
o o o o o o
```

The cartridge is bundled with a default [levels.txt](./levels.txt), but in case other files are created the one to use can be passed as an argument:

```
--file <file>
```

The first level to be played can also be passed as an argument:

```
--level 20
```

This will load the levels file, but will skip the first 20 levels, starting from the 21st.
