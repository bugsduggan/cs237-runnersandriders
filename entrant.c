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

/* turn a status into a string */
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

/* compares entrants (used by the vector to sort entrants) */
int compare_entrants(void* vp1, void* vp2) {
  /*
   * a < b  -  -1
   * a = b  -  0
   * a > b  -  1
   */
  Entrant* a = *(Entrant**)vp1;
  Entrant* b = *(Entrant**)vp2;

  /*
   * Put finished at the top
   * ascending by duration
   *
   * Then started/stopped
   * descending by duration
   *
   * The disqualified
   * descending by duration
   *
   * Then not started
   * ascending by id
   */

  if (a->status == FINISHED) {
    if (b->status == FINISHED) {
      if (a->duration < b->duration) return -1;
      else if (a->duration > b->duration) return 1;
      else return 0;
    } else return -1; /* b is not finished */

  } else if (a->status == STARTED || a->status == STOPPED) {
    if (b->status == FINISHED) return 1;
    else if (b->status == STARTED || b->status == STOPPED) {
      if (a->duration < b->duration) return 1;
      else if (a->duration > b->duration) return -1;
      else return 0;
    } else return -1; /* b is either not started or is disqualified */

  } else if (a->status == DISQUAL_SAFETY || a->status == DISQUAL_INCORR) {
    if (b->status == FINISHED || b->status == STARTED || b->status == STOPPED) return 1;
    else if (b->status == DISQUAL_SAFETY || b->status == DISQUAL_INCORR) {
      if (a->duration < b->duration) return 1;
      else if (a->duration > b->duration) return -1;
      else return 0;
    } else return -1; /* b has not even started */

  } else { /* a has not started */
    if (b->status != NOT_STARTED) return 1;
    else {
      if (a->id < b->id) return -1;
      else return 1; /* ids will not be equal */
    }
  }
}

/* update the last_cp_node and time (and current node and time)
 * of the entrant */
void update_nodes(Entrant* entrant, Node* node, Time* time) {
  entrant->last_cp_node = node;
  if (entrant->last_cp_time) free(entrant->last_cp_time);
  entrant->last_cp_time = timecpy(time);

  entrant->last_node = node;
  if (entrant->last_time) free(entrant->last_time);
  entrant->last_time = timecpy(time);
}

/*
 * functions declared in data.h
 */

/* read entrants from the file */
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

    /* other data 
     * these all get updated as the entrant starts in entrant_update_location */
    entrant->duration = 0;
    entrant->status = NOT_STARTED;
    entrant->start_time = NULL;
    entrant->last_cp_node = NULL;
    entrant->last_cp_time = NULL;
    entrant->last_node = NULL;
    entrant->last_time = NULL;
    entrant->curr_time = NULL;
    entrant->curr_track = NULL;

    Vector_add(entrants, &entrant);
  }

  Vector_dispose(lines);
  return entrants;
}

/* find an entrant by id */
Entrant* entrant_from_id(Vector* entrants, int id) {
  Entrant* entrant;
  int i = 0;

  for (i = 0; i < Vector_size(entrants); i++) {
    Vector_get(entrants, i, &entrant);
    if (entrant->id == id) return entrant;
  }

  return NULL;
}

/* print out the entrants stats */
void entrant_stats(Entrant* entrant, Time* curr_time) {
  /* generic data for all entrants */
  printf("\n");
  printf("\t%3d: %-50s\n", entrant->id, entrant->name);
  printf("\t\tRunning course:         %c\n", entrant->course->id);

  if (entrant->status == NOT_STARTED) {
    printf("\t\tWaiting to start\n");
  } else {
      printf("\t\tStarted at:             %02d:%02d\n",
          entrant->start_time->hours,
          entrant->start_time->minutes);
    /*
     * started
     */
    if (entrant->status == STARTED) {
      printf("\t\tEstimated location:     Track %d\n", entrant->curr_track->id);
      printf("\t\tLast checkpoint:        Node %d at %02d:%02d (%d mins ago)\n",
          entrant->last_cp_node->id,
          entrant->last_cp_time->hours,
          entrant->last_cp_time->minutes,
          (time_to_duration(curr_time) - time_to_duration(entrant->last_cp_time)));
      printf("\t\tRun time:               %d mins\n", entrant->duration);
    /*
     * stopped
     */
    } else if (entrant->status == STOPPED) {
      printf("\t\tAt medical checkpoint:  Node %d since %02d:%02d (%d mins ago)\n",
          entrant->last_cp_node->id, entrant->last_cp_time->hours,
          entrant->last_cp_time->minutes,
          (time_to_duration(curr_time) - time_to_duration(entrant->last_cp_time)));
      printf("\t\tRun time:               %d mins\n", entrant->duration);
    /*
     * disqualified - safety
     */
    } else if (entrant->status == DISQUAL_SAFETY) {
      printf("\t\tExcluded for safety at: Node %d at %02d:%02d\n",
          entrant->last_cp_node->id,
          entrant->last_cp_time->hours,
          entrant->last_cp_time->minutes);
    /*
     * disqualified - bad route
     */
    } else if (entrant->status == DISQUAL_INCORR) {
      printf("\t\tDisqualified at:        Node %d at %02d:%02d\n",
          entrant->last_cp_node->id,
          entrant->last_cp_time->hours,
          entrant->last_cp_time->minutes);
    /*
     * finished
     */
    } else if (entrant->status == FINISHED) {
      printf("\t\tFinished at:            %02d:%02d\n",
          entrant->last_cp_time->hours,
          entrant->last_cp_time->minutes);
      printf("\t\tTotal time:             %d mins\n", entrant->duration);
    }
  }
}

/* update the entrant's location */
void entrant_update_location(Event* event, char type, int entrant_id, int node_id) {
  Entrant* entrant = entrant_from_id(event->entrants, entrant_id);
  Node* node = node_from_id(event->nodes, node_id);

  /* This entrant will not have had entrant_update_time called on it */

  /********************************
   * T type
   ********************************/
  if (type == 't' || type == 'T') {
    /* check if not started and init */
    if (entrant->status == NOT_STARTED) {
      entrant->status = STARTED;
      entrant->start_time = timecpy(event->time);
      entrant->curr_time = timecpy(event->time);
    }

    update_nodes(entrant, node, event->time);

    /* update duration and current time */
    entrant->duration += time_to_duration(event->time) -
      time_to_duration(entrant->curr_time);
    if (entrant->curr_time) free(entrant->curr_time);
    entrant->curr_time = timecpy(event->time);

    /* update current track */
    if (entrant->curr_track)
      entrant->curr_track =
        next_track_from_node(entrant->course, entrant->curr_track, node);
    else
      /* _might_ not be inited (on starting) so init to tracks[0] */
      Vector_get(entrant->course->tracks, 0, &entrant->curr_track);

    /* check if finished */
    if (entrant->curr_track == NULL)
      entrant->status = FINISHED;

  /********************************
   * I type
   ********************************/
  } else if (type == 'i' || type == 'I') {
    update_nodes(entrant, node, event->time);

    /* update duration and current time */
    entrant->duration += time_to_duration(event->time) -
      time_to_duration(entrant->curr_time);
    if (entrant->curr_time) free(entrant->curr_time);
    entrant->curr_time = timecpy(event->time);

    entrant->status = DISQUAL_INCORR;

  /********************************
   * A type
   ********************************/
  } else if (type == 'a' || type == 'A') {
    update_nodes(entrant, node, event->time);

    entrant->duration += time_to_duration(event->time) -
      time_to_duration(entrant->curr_time);

    entrant->status = STOPPED;

  /********************************
   * D type
   ********************************/
  } else if (type == 'd' || type == 'D') {
    update_nodes(entrant, node, event->time);

    /* set current time */
    if (entrant->curr_time) free(entrant->curr_time);
    entrant->curr_time = timecpy(event->time);

    /* update next track and status */
    entrant->curr_track =
      next_track_from_node(entrant->course, entrant->curr_track, node);
    entrant->status = STARTED;

  /*******************************
   * E type
   *******************************/
  } else { /* type == 'e' */
    entrant->status = DISQUAL_SAFETY;
  }
}

/* update the entrant's time and estimated location */
void entrant_update_time(Event* event, Entrant* entrant) {
  int time_since_seen; /* time since last node */
  int time_delta; /* time since this function was last called */

  if (entrant->status == STARTED) {
    /* if they've started, they'd better not have null values */

    /* calc some useful values */
    time_since_seen = time_to_duration(event->time) -
      time_to_duration(entrant->last_time);
    time_delta = time_to_duration(event->time) -
      time_to_duration(entrant->curr_time);

    /* update duration and curr_time */
    entrant->duration += time_delta;
    if (entrant->curr_time) free(entrant->curr_time);
    entrant->curr_time = timecpy(event->time);

    if (entrant->curr_track->end->type == JN && /* next junction is not timed */
        time_since_seen > entrant->curr_track->safe_time) { /* entrant has finished this track */
      /* find next track */
      entrant->curr_track = next_track(entrant->course, entrant->curr_track);
      /* update last node & time */
      entrant->last_node = entrant->curr_track->start;
      if (entrant->last_time) free(entrant->last_time);
      entrant->last_time = timecpy(event->time);
    }
  }
}

/* sort the entrants */
void entrants_sort(Event* event) {
  Vector_sort(event->entrants, compare_entrants);
}
