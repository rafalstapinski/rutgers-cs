// Author: Rafal Stapinski

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter_server.h"

struct Row *msort(struct Row *head, const char *column) {

    struct Row *one = head;

    // if already sorted (one el) return

    if ((one == NULL) || (one->next == NULL)) {
        return one;
    }

    // cut in two, and recurse

    struct Row *two = split_list(one);
    return merge_list(msort(one, column), msort(two, column), column);
}

struct Row* merge_list(struct Row *one, struct Row *two, const char *column) {

    struct Row temp;
    struct Row *end = &temp;

    // do comparisons to find min

    while ((one != NULL) && (two != NULL)) {

      struct Row **min = NULL;

        if (strcmp(column, "color") == 0) {
          if (strcmp((one->color), (two->color)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "director_name") == 0) {
          if (strcmp((one->director_name), (two->director_name)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "num_critic_for_reviews") == 0) {
          if (*(one->num_critic_for_reviews) < *(two->num_critic_for_reviews)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "duration") == 0) {
          if (*(one->duration) < *(two->duration)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "director_facebook_likes") == 0) {
          if (*(one->director_facebook_likes) < *(two->director_facebook_likes)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "actor_3_facebook_likes") == 0) {
          if (*(one->actor_3_facebook_likes) < *(two->actor_3_facebook_likes)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "actor_2_name") == 0) {
          if (strcmp((one->actor_2_name), (two->actor_2_name)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "actor_1_facebook_likes") == 0) {
          if (*(one->actor_1_facebook_likes) < *(two->actor_1_facebook_likes)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "gross") == 0) {
          if (*(one->gross) < *(two->gross)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "genres") == 0) {
          if (strcmp((one->genres), (two->genres)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "actor_1_name") == 0) {
          if (strcmp((one->actor_1_name), (two->actor_1_name)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "movie_title") == 0) {
          if (strcmp((one->movie_title), (two->movie_title)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "num_voted_users") == 0) {
          if (*(one->num_voted_users) < *(two->num_voted_users)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "cast_total_facebook_likes") == 0) {
          if (*(one->cast_total_facebook_likes) < *(two->cast_total_facebook_likes)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "actor_3_name") == 0) {
          if (strcmp((one->actor_3_name), (two->actor_3_name)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "facenumber_in_poster") == 0) {
          if (*(one->facenumber_in_poster) < *(two->facenumber_in_poster)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "plot_keywords") == 0) {
          if (strcmp((one->plot_keywords), (two->plot_keywords)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "movie_imdb_link") == 0) {
          if (strcmp((one->movie_imdb_link), (two->movie_imdb_link)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "num_user_for_reviews") == 0) {
          if (*(one->num_user_for_reviews) < *(two->num_user_for_reviews)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "language") == 0) {
          if (strcmp((one->language), (two->language)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "country") == 0) {
          if (strcmp((one->country), (two->country)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "content_rating") == 0) {
          if (strcmp((one->content_rating), (two->content_rating)) < 0) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "budget") == 0) {
          if (*(one->budget) < *(two->budget)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "title_year") == 0) {
          if (*(one->title_year) < *(two->title_year)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "actor_2_facebook_likes") == 0) {
          if (*(one->actor_2_facebook_likes) < *(two->actor_2_facebook_likes)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "imdb_score") == 0) {
          if (*(one->imdb_score) < *(two->imdb_score)) {
            min = &one;
          } else {
            min = &two;
          }
        } else if (strcmp(column, "aspect_ratio") == 0) {
          if (*(one->aspect_ratio) < *(two->aspect_ratio)) {
            min = &one;
          } else {
            min = &two;
          }
        } else {
          if (*(one->movie_facebook_likes) < *(two->movie_facebook_likes)) {
            min = &one;
          } else {
            min = &two;
          }
        }

        struct Row *next = (*min)->next;
        end = end->next = *min;
        *min = next;
    }
    if (!(end->next = one)) {
      end->next = two;
    }
    return temp.next;
}

struct Row *split_list(struct Row *head) {
    struct Row *half = head;
    struct Row *end = head;
    struct Row *prev = NULL;

    // walk to end to find middle

    while ((end != NULL) && (end->next != NULL)) {
        prev = half;
        half = half->next;
        end = end->next->next;
    }
    if (prev != NULL) {
        prev->next = NULL;
    }
    return half;
}
