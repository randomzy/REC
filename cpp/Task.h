#ifndef TASK_H
#define TASK_H

template<typename Image>
struct Task
{
  Image * image;
  int ystart;
  int ysize;
};

#endif /* TASK_H */
