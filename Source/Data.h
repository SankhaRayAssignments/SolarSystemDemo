#define EARTH_RADIUS 1.0f
#define EARTH_ROTATION_SPEED 7.2f
#define EARTH_REVOLUTION_SPEED 0.02f
#define EARTH_SUN_DIST 200.0f

#define SUN_RADIUS 10.0f
#define SUN_ROTATION_SPEED 0.0f
#define SUN_REVOLUTION_SPEED 0.0f 

#define MOON_RADIUS 0.33f
#define MOON_ROTATION_SPEED 0.267f
#define MOON_REVOLUTION_SPEED 0.267f 
#define EARTH_MOON_DIST 5.0f

enum ESolarType
{
	SUN = 0,
	EARTH = 1,
	MOON = 2,
	FORCEDWORD1 = 0x7fffffff
};
