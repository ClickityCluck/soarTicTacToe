#include "tic_tac_toe.h"

#include <curl/curl.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>

static char const *const DEFAULT_ADDRESS = "127.0.0.1";
static char const *const DEFAULT_PORT = "8000";

typedef struct _ttt_t {
  ttt_state_t state;
  CURL *curl;
  char const *address;
  char const *port;
} ttt_t;

size_t write_callback(char *buf, size_t size, size_t nmemb, void *up) {
  size_t ret_val = 0;
  ttt_state_t *state = (ttt_state_t *)up;
  json_object *board = NULL;

  json_object *obj = json_tokener_parse(buf);
  if (!obj) {
    goto end;
  }

  // Parse board

  if (!json_object_object_get_ex(obj, "board", &board)) {
    goto end;
  }
  size_t board_length = json_object_array_length(board);
  if (board_length != 9) {
    fprintf(stderr, "Board length should be 9 but it is %ld\n", board_length);
    goto end;
  }
  for (size_t i = 0; i != 9; ++i) {
    json_object *cell_obj = json_object_array_get_idx(board, i);
    char const *cell_str = json_object_get_string(cell_obj);
    state->board[i] = cell_str ? *cell_str : 0;
  }

  // Parse turn

  json_object *turn_json = NULL;
  if (!json_object_object_get_ex(obj, "turn", &turn_json)) {
    goto end;
  }
  char const *turn_str = json_object_get_string(turn_json);
  state->turn = turn_str ? *turn_str : 0;

  // Parse winner

  json_object *winner_json = NULL;
  if (!json_object_object_get_ex(obj, "winner", &winner_json)) {
    goto end;
  }
  char const *winner_str = json_object_get_string(winner_json);
  state->winner = winner_str ? *winner_str : 0;

  // Cleanup

  ret_val = size * nmemb;

end:
  if (obj && !json_object_put(obj)) {
    fprintf(stderr, "JSON object wasn't freed\n");
  }

  return ret_val;
}

ttt_t *ttt_init() {
  ttt_t *ttt = calloc(1, sizeof(ttt_t));
  if (!ttt) {
    goto fail;
  }
  for (int i = 0; i != 9; ++i) {
    ttt->state.board[i] = '\0';
  }
  ttt->state.turn = '\0';
  ttt->state.winner = '\0';

  ttt->curl = curl_easy_init();
  if (!ttt->curl) {
    goto fail;
  }

  ttt->address = getenv("TTT_ADDRESS");
  if (!ttt->address) {
    ttt->address = DEFAULT_ADDRESS;
  }

  ttt->port = getenv("TTT_PORT");
  if (!ttt->port) {
    ttt->port = DEFAULT_PORT;
  }

  return ttt;

fail:
  ttt_destroy(ttt);
  return NULL;
}

void ttt_destroy(ttt_t *ttt) {
  if (ttt) {
    if (ttt->curl) {
      curl_easy_cleanup(ttt->curl);
    }
    free(ttt);
  }
}

ttt_state_t *ttt_get_state(ttt_t *ttt) {
  char url[128];
  sprintf(url, "%s:%s/ttt/state", ttt->address, ttt->port);
  curl_easy_setopt(ttt->curl, CURLOPT_URL, url);
  curl_easy_setopt(ttt->curl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(ttt->curl, CURLOPT_WRITEFUNCTION, &write_callback);
  curl_easy_setopt(ttt->curl, CURLOPT_WRITEDATA, &ttt->state);
  CURLcode res = curl_easy_perform(ttt->curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    return NULL;
  }
  return &ttt->state;
}

void ttt_make_move(ttt_t *ttt, ttt_move_t move) {
  char url[128];
  sprintf(url, "%s:%s/ttt/play?player=%c&x=%d&y=%d", ttt->address, ttt->port,
          move.player, move.x, move.y);
  curl_easy_setopt(ttt->curl, CURLOPT_URL, url);
  curl_easy_setopt(ttt->curl, CURLOPT_POSTFIELDS, "");
  CURLcode res = curl_easy_perform(ttt->curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
}

void ttt_reset(ttt_t *ttt) {
  char url[128];
  sprintf(url, "%s:%s/ttt/reset", ttt->address, ttt->port);
  curl_easy_setopt(ttt->curl, CURLOPT_URL, url);
  curl_easy_setopt(ttt->curl, CURLOPT_POSTFIELDS, "");
  curl_easy_setopt(ttt->curl, CURLOPT_WRITEFUNCTION, NULL);
  CURLcode res = curl_easy_perform(ttt->curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
}
