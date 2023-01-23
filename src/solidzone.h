#ifndef SOLIDZONE
#define SOLIDZONE

// Main Game Zone
static const float ZONESIZE = 55.0f; // outside zone

static const float HOUSESIZE = 10.1f;

static const float ZONEDX[] = {
    -10.0f, -2.5f,  //orange left wall
    -0.78f, 7.6f,   //blue wall
    -2.49f, -0.77f, //orange door
    7.61, 10.0f,    //blue door
    .03f, 10.0f,    //purple wall
    -0.15f, 0.11f,  // in between orange blue rooms wall
    5.7f, 6.2f,     // red wall
    5.7f, 10.0f     // red destructable wall

};

static const float ZONEDZ[] = {
    -4.8f, -5.2f,  //orange left wall and blue wall
    -7.8f, -8.2f,  // purple wall
    -4.8f, -10.0f, // in between orange and blue rooms wall
    10.0f, 2.7f,   // red wall
    2.8f, 2.6f,    // red destructable wall
};

#endif // SOLIDZONE
