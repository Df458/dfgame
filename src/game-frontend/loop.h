#ifndef DF_LOOP_H
#define DF_LOOP_H

/*!
 * This is a function that runs for each main loop iteration.
 */
typedef void (*loop_func)(float delta_time);

/*!
 * This function creates a simple main loop, and runs f each iteration. To exit
 * the main loop, call main_loop_request_end().
 */
void main_loop_begin(loop_func f);

/*!
 * This tells the running main loop to exit instead of continuing to iterate.
 * It doesn't exit immediately, instead waiting until the current iteration
 * finishes.
 */
void main_loop_request_end();

#endif
