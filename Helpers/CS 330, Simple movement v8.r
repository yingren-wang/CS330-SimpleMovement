# Project:  CS 330
# Program:  Simple movement
# Purpose:  Implement simple movement update algorithms (section 3.1).
# Author:   Mikel D. Petty, Ph.D., 256-824-6140, pettym@uah.edu
# Created:  2019-1-17
# Modified: 2020-1-25
# Source:   I. Millington and J. Funge, Artficial Intelligence for Games,
#           Third Edition, CRC Press, Boca Raton FL, 2019.

# Clear workspace.

rm(list = ls())

# Initialize file paths and names.

location        <- 1  # 1=OKT N353, 2=27CPD
work.paths      <- c("C:/Users/mpetty/Desktop/Working, CS 330/",
                     "C:/Users/Barbara J. Schiavone/Desktop/Working, CS 330/")
work.path       <- work.paths[location]
trajectory.file <- paste(work.path, "CS 330, Simple trajectory data.txt", sep="")

# Define utility functions.

text.out  <- function(msg, textfile, first=FALSE) { write(msg, textfile, ncolumns=1, append=!first) }

# Initialize character movement variables.

position    <- c(0, 60) # Location in game world; 2D vector; meters
velocity    <- c(8, 2)  # Rate of change of position; 2D vector; meters per second
linear      <- c(0, 1)  # Rate of change of velocity; 2D vector; meters per second per second
orientation <- 0        # Direction character is facing; numeric; radians
rotation    <- 0        # Rate of change of orientation; numeric; radians per second
angular     <- 0.00     # Rate of change of rotation; numeric; radians per second per second

# Initialize movement parameters.

Time       <- 0       # Current simulated time
delta.time <- 0.50    # Duration of time step
physics    <- FALSE   # TRUE=HS physics, FALSE=Newton-Euler-1 integration (p. 47)
align      <- FALSE   # TRUE=align character orientation with velocity, FALSE=do not align

# Write initial position and movement variables to trajectory file.

initial <- paste(Time, position[1], position[2], velocity[1], velocity[2], linear[1], linear[2], orientation, rotation, angular, sep=",")
text.out(initial, trajectory.file, first=TRUE)

update.HS <- function(position, velocity, linear, orientation, rotation, angular) {

  # Update position and orientation.

  half.t.sq   <- 0.5 * delta.time * delta.time
  position    <- position + (velocity * delta.time) + (linear * half.t.sq)
  orientation <- orientation + (rotation * delta.time) + (angular * half.t.sq)

  # Update velocity and rotation.

  velocity <- velocity + (linear * delta.time)
  rotation <- rotation + (angular * delta.time)

  updated <- list(position, velocity, orientation, rotation)
  return(updated)
}

update.NE1 <- function(position, velocity, linear, orientation, rotation, angular) {

  # Update position and orientation.

  position <- position + (velocity * delta.time)
  orientation <- orientation + (rotation * delta.time)

  # Update velocity and rotation.

  velocity <- velocity + (linear * delta.time)
  rotation <- rotation + (angular * delta.time)

  updated <- list(position, velocity, orientation, rotation)
  return(updated)
}

# Calculate trajectory, time step by time step.

while (Time < 100) {
  Time <- Time + delta.time

  # Update character movement variables.

  if (physics) {
    updated <- update.HS(position, velocity, linear, orientation, rotation, angular)   # High School physics
  } else {
    updated <- update.NE1(position, velocity, linear, orientation, rotation, angular)  # Newton-Euler-1 integration
  }

  position    <- updated[[1]]
  velocity    <- updated[[2]]
  orientation <- updated[[3]]
  rotation    <- updated[[4]]

  if (align) { orientation <- atan2(velocity[2], velocity[1]) }

  # Set linear and angular acceleration.
  # This is a surrogate steering behavior, for testing only.

  linear  <- c(position[1] / -100 * delta.time, position[2] / -100 * delta.time) * c(9, 3)
  angular <- (50 - Time) * 0.0001

  # Write updated position and movement variables to trajectory file.

  updated <- paste(Time, position[1], position[2], velocity[1], velocity[2], linear[1], linear[2], orientation, rotation, angular, sep=",")
  text.out(updated, trajectory.file)
}

# End of program
