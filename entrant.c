/*
 * entrant
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * private functions
 */

char* status_to_str(entrant_status status) {
  if (status == NOT_STARTED) {
    return "Waiting to start";
  } else if (status == STARTED) {
    return "Started";
  } else if (status == STOPPED) {
    return "At medical checkpoint";
  } else if (status == DISQUAL_SAFETY) {
    return "Disqualified for safety";
  } else if (status == DISQUAL_INCORR) {
    return "Disqualified for incorrect route";
  } else {
    return "Finished";
  }
}

/* TODO */
int compare_entrants(void* vp1, void* vp2) {
  /*
   * a < b  -  -1
   * a = b  -  0
   * a > b  -  1
   */
  Entrant* a = *(Entrant**)vp1;
  Entrant* b = *(Entrant**)vp2;

  if (a->status == NOT_STARTED) {
    if (b->status == NOT_STARTED) {
      /* neither have started, compare by id */
      if (a->id < b->id) return -1;
      else if (a->id > b->id) return 1;
      else return 0;
    } else {
      /* b has at least started */
      return 1;
    }
  } else if (a->status == FINISHED) {
    if (b->status == FINISHED) {
      /* both have finished, compare by duration */
      if (a->duration < b->duration) return -1;
      else if (a->duration > b->duration) return 1;
      else return 0;
    } else {
      /* b is still out there somewhere */
      return -1;
    }
  } else { /* a is out on the course somewhere */
    if (b->status == NOT_STARTED) {
      return -1;
    } else if (b->status == FINISHED) {
      return 1;
    } else {
      /* both out on the course */
      /* what I'd like to do is work out who is closer to finishing by what track they're on */
      /* but for now, whoever's been going the longest can be nearest the top */
      if (a->duration < b->duration) return 1;
      else if (a->duration > b->duration) return -1;
      else return 0;
    }
  }
}

/*
 * functions declared in data.h
 */

Vector* read_entrants(char* filename, Vector* courses) {
  Vector* lines = read_file(filename);
  Vector* entrants = Vector_new(sizeof(Entrant*), NULL);
  Entrant* entrant;
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    entrant = malloc(sizeof(Entrant));

    /* id */
    token = strtok(line, " ");
    entrant->id = atoi(token);

    /* course */
    token = strtok(NULL, " ");
    entrant->course = course_from_id(courses, token[0]); /* 1 char */

    /* name */
    token = strtok(NULL, "\n"); /* read to end of line */
    entrant->name = strdup(token);

    /* other data */
    entrant->duration = 0;
    entrant->status = NOT_STARTED;
    entrant->start_time = NULL;
    entrant->last_cp_node = NULL;
    entrant->last_cp_time = NULL;
    entrant->last_node = NULL;
    entrant->last_time = NULL;
    entrant->curr_track = NULL;

    Vector_add(entrants, &entrant);
  }

  Vector_dispose(lines);
  return entrants;
}

Entrant* entrant_from_id(Vector* entrants, int id) {
  Entrant* entrant;
  int i = 0;

  for (i = 0; i < Vector_size(entrants); i++) {
    Vector_get(entrants, i, &entrant);
    if (entrant->id == id) return entrant;
  }

  return NULL;
}

void entrant_stats(Entrant* entrant, Time* curr_time) {
  printf("\n");
  printf("\t%2d: %-50s\n", entrant->id, entrant->name);
  printf("\t\tRunning course:     %c\n", entrant->course->id);
  if (entrant->status == NOT_STARTED) {
    printf("\t\tWaiting to start\n");
  } else if (entrant->status == STARTED) {
    printf("\t\tStarted at:         %02d:%02d\n", entrant->start_time->hours, entrant->start_time->minutes);
    printf("\t\tEstimated location: Track %d\n", entrant->curr_track->id);
    printf("\t\tLast checkpoint:    Node %d at %02d:%02d (%d mins ago)\n",
        entrant->last_cp_node->id, entrant->last_cp_time->hours,
        entrant->last_cp_time->minutes, (time_to_duration(curr_time) - time_to_duration(entrant->last_cp_time)));
    printf("\t\tRun time:           %d mins\n", entrant->duration);
  } else if (entrant->status == STOPPED) {
    /* TODO */
  } else if (entrant->status == DISQUAL_SAFETY) {
    /* TODO */
  } else if (entrant->status == DISQUAL_INCORR) {
    /* TODO */
  } else if (entrant->status == FINISHED) {
    printf("\t\tStarted at:         %02d:%02d\n", entrant->start_time->hours, entrant->start_time->minutes);
    printf("\t\tFinished at:        %02d:%02d\n", entrant->last_cp_time->hours, entrant->last_cp_time->minutes);
    printf("\t\tTotal time:         %d mins\n", entrant->duration);
  }
}

/* TODO */
void entrant_update_location(Event* event, int entrant_id, int node_id) {
  Entrant* entrant = entrant_from_id(event->entrants, entrant_id);
  Node* node = node_from_id(entrant->course->nodes, node_id);

  if (entrant->status == NOT_STARTED) {
    entrant->status = STARTED;
    entrant->start_time = timecpy(event->time);
  }

  entrant->last_cp_node = node;
  if (entrant->last_cp_time) free(entrant->last_cp_time);
  entrant->last_cp_time = timecpy(event->time);

  entrant->last_node = entrant->last_cp_node;
  if (entrant->last_time) free(entrant->last_time);
  entrant->last_time = timecpy(entrant->last_cp_time);

  entrant->duration = time_to_duration(event->time) -
    time_to_duration(entrant->start_time);

  if (entrant->curr_track) {
    /* update the track with the next one */
    entrant->curr_track = next_track_from_node(entrant->course, entrant->curr_track, node);
  } else {
    /* entrant must have just started, curr_track = tracks[0] */
    Vector_get(entrant->course->tracks, 0, &entrant->curr_track);
  }

  if (entrant->curr_track == NULL) {
    entrant->status = FINISHED;
  }
}

/* TODO */
void entrant_update_time(Event* event, Entrant* entrant) {
  int last_seen;
  if (entrant->status == STARTED || entrant->status == STOPPED) {
    /* update duration */
    entrant->duration = time_to_duration(event->time) -
      time_to_duration(entrant->start_time);
    /* check where entrant is */
    last_seen = time_to_duration(event->time) -
      time_to_duration(entrant->last_time);
    if (entrant->curr_track->end->type == JN && /* next junction is not timed */
        last_seen > entrant->curr_track->safe_time) { /* entrant has probably finished this track */
      /* update the track */
      entrant->curr_track = next_track(entrant->course, entrant->curr_track);
      entrant->last_node = entrant->curr_track->start;
      if (entrant->last_time) free(entrant->last_time);
      entrant->last_time = timecpy(event->time);
    }
  }
}

void entrants_sort(Event* event) {
  Vector_sort(event->entrants, compare_entrants);
}
