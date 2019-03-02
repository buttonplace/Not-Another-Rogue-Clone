#ifndef COORDINATES_H
# define COORDINATES_H

typedef enum coordinate {
  dim_x,
  dim_y,
  num_dims
} coordinate_t;

typedef int8_t pair_t[num_dims];

#endif
