// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include <unistd.h>
#include "mongoose.h"

static const char *s_listen_on = "http://0.0.0.0:8000";
char s_web_directory[100];
// HTTP request handler function. It implements the following endpoints:
//   /upload - Saves the next file chunk
//   /api/video1 - hangs forever, returns MJPEG video stream
//   all other URI - serves web_root/ directory
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/upload")) {
      mg_http_upload(c, hm, "/tmp");
    } else if (mg_http_match_uri(hm, "/api/video1")) {
      c->label[0] = 'S';  // Mark that connection as live streamer
      mg_printf(
          c, "%s",
          "HTTP/1.0 200 OK\r\n"
          "Cache-Control: no-cache\r\n"
          "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
          "Content-Type: multipart/x-mixed-replace; boundary=--foo\r\n\r\n");
    }
    else {
      mg_http_serve_dir(c, ev_data, s_web_directory);
    }
  }
  (void) fn_data;
}

// The image stream is simulated by sending MJPEG frames specified by the
// "files" array of file names.
static void broadcast_mjpeg_frame(struct mg_mgr *mgr) {
  const char *files[] = {"images/1.jpg", "images/2.jpg", "images/3.jpg",
                         "images/4.jpg", "images/5.jpg", "images/6.jpg"};
  size_t nfiles = sizeof(files) / sizeof(files[0]);
  static size_t i;
  char path[100];
  strcpy(path, s_web_directory);
  strcat(path, files[i++ % nfiles]);
  printf("image path: %s\n", path);
  size_t size = mg_file_size(path);
  char *data = mg_file_read(path);  // Read next file
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->label[0] != 'S') continue;      // Skip non-stream connections
    if (data == NULL || size == 0) continue;  // Skip on file read error
    mg_printf(c,
              "--foo\r\nContent-Type: image/jpeg\r\n"
              "Content-Length: %lu\r\n\r\n",
              (unsigned long) size);
    mg_send(c, data, size);
    mg_send(c, "\r\n", 2);
  }
  free(data);
}

static void timer_callback(void *arg) {
  broadcast_mjpeg_frame(arg);
}

int main(int argc, char **argv) {
  struct mg_mgr mgr;
  struct mg_timer t1;

  int opt;
  strcpy(s_web_directory, "web_root");
  while ((opt = getopt(argc, argv, "d:")) != -1) {
    switch (opt) {
    case 'd':
        strcpy(s_web_directory, optarg);
        break;
    default: /* '?' */
        fprintf(stderr, "Usage: %s [-d web_dir (default: web_root)]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  mg_mgr_init(&mgr);
  mg_log_set("3");
  mg_http_listen(&mgr, s_listen_on, cb, NULL);
  mg_timer_init(&t1, 500, MG_TIMER_REPEAT, timer_callback, &mgr);
  for (;;) mg_mgr_poll(&mgr, 50);
  mg_timer_free(&t1);
  mg_mgr_free(&mgr);

  return 0;
}
