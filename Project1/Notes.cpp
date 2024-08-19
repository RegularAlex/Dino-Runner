//Check where the player character ends up whilst moving, something is wrong there. 
// - Some randomly big negative numbers appear. (Fixed the updating the axis was in the wrong position)
//Need to Change the movement when it works so it is done automatically (constantly running) 
// - Constantly moving to the right, until the user collides with an obstacle. (Works, but not effectively needs to be upgraded at some point)
//Need to make it so the ground is physical and the player cant pass through it. 
// - Or maybe make it so the player spawns on the ground and when they jump they return to the same point
//Need to add obstacles and collisions. 
// - One for ducking and one for jumping (completed there is a cactus for the ground obstacle and a flock of birds for the air obstacle)
// - Wall need to act as removal zones, when something touches a wall it needs to be removed (and if needed replaced) (completed the left hand wall acts as the removal as soon as something touches it, that item is removed)
//Need to add a functioning score aspect. 
// - Points for distance travelled (There is a point added each moment the player surives)
// - Points for successfully dodging obstacles?
//Need to make it endless 
// - Looping background and ground, infinity spawning obstacles (works, ground is looping one after another and the obstacles are randomly appearing)
//Need to store the high score and display it on the front screen
// - Saved in an external text file 
//After a playthrough return the user back the the main menu
// - With it mostly being the same, the only difference is that the users score is displayed. (completed the players score is displayed on the main menu)