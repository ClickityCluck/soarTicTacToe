---
title: Tic Tac Toe
---

Let's make a Soar agent play tic tac toe!

Soar agents aren't very interesting unless they can interact with the
outside world, so we've provided a game for it to interact with.

# Running the game

To run the game server, just execute:

```bash
$ ./ttt-server
```

This will launch a web server. Point your browser at `localhost:8000`,
and you'll be able to play the game. When your agent makes moves, they
will be reflected here.

You can change the address and port of the game server by passing
command line options. Run with the `-h` flag for more information.

# Running the agent

We've provided an API for interacting with the game server. Look at
`./tic_tac_toe.h` for details - it's a fairly simple interface.

We've also provided a starter program and a Makefile that exercises
the game's API. With the server running, execute the following:

```bash
$ sudo apt-get update
$ sudo apt-get install make pkg-config libcurl4-gnutls-dev libjson-c-dev
$ make
$ ./ttt-agent
```

If everything worked correctly, the starter program should make a
hardcoded move, which will be reflected on the board in your web
browser.

The tic-tac-toe API needs to know the address and port of the game
server. If you are not using the defaults, you can configure these by
setting the `TTT_ADDRESS` and `TTT_PORT` environment variables
respectively.

# Your challenge

In order to make a Soar agent play tic-tac-toe, you'll need to:

- Design an agent that can play tic-tac-toe.
- Modify the Makefile to compile and link Soar.
- Design the structure of input- and output-links.
- Use the API in `tic_tac_toe.h` to query the game state and update the input-link.
- Handle your agent's outputs and use the game's API to affect the game.
