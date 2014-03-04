/*============================================================================*/
/* 
 * Bread Man
 * A simple game played with a frozen loaf of bread, a warm body, and a 
 * thermister. 
 * Copyright (C) 2014 Philip Garrison
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, 
 * USA.
 *
 *
 * An 8x8 LED matrix displays your current position on the left
 * side of the screen, and obstacles fall from the right side of the screen.
 * Your position is determined by the termperature of the thermister - hotter
 * is up, colder is down. For each obstacle (or pair of obstacles, in hard 
 * mode) that the player avoids, the player gets a point. The player has three
 * lives, which they lose by hitting obstacles. When all three lives are lost,
 * the game is over and the score is displayed.
 * 
 * There is also a musical accompaniment, including a title theme (the intro to
 * the first gen Pokemon wild battle music), a main theme (the body of said wild
 * battle music), and various sound effects.
 * 
 *
 * Credits:
 *   All of the (non-original) music comes from Wild Pokemon Battle, composed by
 *   Junichi Masuda and arranged by bespinben. The sheet music is available at
 *     https://dl.dropboxusercontent.com/u/35209091/RBY%20Wild%20Pokemon%20Battle.pdf
 * 
 *   The note frequencies come from 
 *     https://en.wikipedia.org/wiki/Piano_key_frequencies
 *   and
 *     http://arduino.cc/en/Tutorial/tone
 */
/*============================================================================*/


/*------- Hardware configuration -------*/
#define THERMAL_PIN A7
#define AUDIO_PIN 12
const int ledrow[8] = {A5, A4, A0, 7, 13, 5, 4, A2};
const int ledcol[8] = {10, A3, 9, A1, 2, 8, 3, 6};


/*------- Music definitions -------*/
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_G6 1568
#define NOTE_F6 1397
#define NOTE_F7 2794

// The length of a sixteenth note, in milliseconds
#define SIXTEENTH 70


/*------- Huge music/display constants -------*/
// For the title screen
const int BreadMan[8][8][8] = {
    { // B
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0}
    },
    { // r
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0}
    },
    { // e
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0}
    },
    { // a
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0}
    },
    { // d
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0}
    },
    { // M
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 1, 0, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 0}
    },
    { // a
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0}
    },
    { // n
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0}
    }
};

// The main music in the game. Copied from the sheet music at
// https://dl.dropboxusercontent.com/u/35209091/RBY%20Wild%20Pokemon%20Battle.pdf
#define NUM_NOTES 144
const int MUSIC[NUM_NOTES] = {
    // 4 bars
    NOTE_G5, NOTE_G5, 
    0, NOTE_E4, 0, 0, NOTE_DS4, NOTE_DS4,
    0, 0, 0, 0, NOTE_CS4, NOTE_CS4, 0, 0,
    0, 0, 0, NOTE_E4, 0, 0, NOTE_DS4, NOTE_DS4,
    0, 0, 0, 
    NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, 

    // 4 bars
    NOTE_G4, NOTE_G4,
    0, NOTE_E4, 0, 0, NOTE_DS4, NOTE_DS4,
    0, 0, 0, 0, NOTE_CS4, NOTE_CS4, 0, 0,
    0, 0, 0, NOTE_E4, 0, 0, NOTE_DS4, NOTE_DS4,
    0, 0, 0, 
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, 

    NOTE_G4, NOTE_G4, NOTE_G4, 
    NOTE_FS4, NOTE_FS4, NOTE_FS4, 
    NOTE_DS4, NOTE_DS4,

    NOTE_G4, NOTE_G4, NOTE_G4, 
    NOTE_A4, NOTE_A4, NOTE_A4,
    NOTE_G4, NOTE_G4,

    // 2 bars
    NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, 
    NOTE_G4, NOTE_G4,
    NOTE_GS4, NOTE_G4, 0, 0, 
    NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, 

    // Begin page 2
    NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_AS4, NOTE_AS4, NOTE_AS4,
    NOTE_GS4, NOTE_GS4,

    NOTE_CS5, NOTE_CS5, NOTE_CS5,
    NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_AS4, NOTE_AS4,

    NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_D5, NOTE_D5,

    NOTE_AS4, NOTE_AS4,
    NOTE_C5, NOTE_C5,
    NOTE_D5, NOTE_D5,
    NOTE_F5, NOTE_F5,

    // 2 bars
    NOTE_GS5, NOTE_GS5, NOTE_GS5, NOTE_GS5, 
    NOTE_GS5, NOTE_GS5, NOTE_GS5, NOTE_GS5, 
    0, 0, 0, 0, 0, 0, 0, 0
};

// For displaying the score
const int DIGITS[10][5][3] = {
    { // Zero
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1}
    },
    { // One
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    },
    { // Two
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1}
    },
    { // Three
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}
    },
    { // Four
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1}
    },
    { // Five
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}
    },
    { // Six 
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    },
    { // Seven
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    { // Eight
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    { // Nine
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
    }
};


/*------- Game state configurations -------*/
// The entire game speed is based on the music tempo, allowing the music to run
// in the same process without disturbing the display. This number is the length
// of time between clock ``ticks''
#define CLOCK_INTERVAL (2*SIXTEENTH) 

#define STARTING_LIVES 3

// Horizontal distance between obstacles
#define OBSTACLE_DISTANCE 4

// Number of milliseconds to display the... display on each cycle. The longer 
// this is, the brighter the display, but the higher the chance of flickering.
// This is determined by experimentation.
#define DISPLAY_INTERVAL 10

// How many milliseconds to display each step of the score animation for.
#define SCORE_ANIMATION_INTERVAL 50

// How long to display the final score (in ms)
#define SCORE_DISPLAY_TIME 1000


/*------- Dynamic game state information -------*/
// When was the last clock tick?
unsigned long lastClock = 0; 

// Each clock tick is one turn.
int turnNumber = 0;

int lives = STARTING_LIVES;

// Measure relative to this temperature.
int thermBasepoint = 500; 

// For use in setDisplay to make the player's position brighter.
int prevrow = 0;

// How many turns have there been since the game was turned on? This is used to
// seed the PRNG
int totalLifetimeTurns = 0;

// In hard mode, there are twice as many obstacles
int hardMode = 0;

// Each element of this array represents the obstacles in a column. Only the 
// last 8 bits of each element are used. If the ith smallest bit in the jth 
// element is set, then the ith row of the jth column has an obstacle.
int obstacles[8] = {0, 0, 0, 0, 0, 0, 0, 0};


/*------- The meat of the program: functions! -------*/
/* 
 * Initializization code (run once via call from Arduino framework).
 * Sets up the hardware and miscellaneous other settings before loading the 
 * title screen
 */
void setup() {
    // Establish direction of pins we are using to drive LEDs 
    for(int i = 0; i < 8; i++) {
        pinMode(ledrow[i], OUTPUT);
        pinMode(ledcol[i], OUTPUT);
    }

    // Clear the screen
    allOff();

    // Seed the PRNG with temperature data, since it's the only analog pin we 
    // aren't using for output.
    randomSeed(analogRead(THERMAL_PIN));

    // Start the player three pixels away from the top of the screen
    thermBasepoint = getTemp() - 3;

    // All ready? Play the intro and start the game!
    titleScreen();
}

/*
 * Turns all of the rows and all of the columns off.
 */
void allOff() {
    for(int i = 0; i < 8; i++) {
        digitalWrite(ledrow[i], HIGH);
        digitalWrite(ledcol[i], LOW);
    }
}

/*
 * The opposite of allOff, turns all of the rows and all of the columns on.
 */
void allOn() {
    for(int i = 0; i < 8; i++) {
        digitalWrite(ledrow[i], LOW);
        digitalWrite(ledcol[i], HIGH);
    }
}

/*
 * Gets a value for the termperature by making two measurements at the 
 * thermister and averaging them to reduce jitter in the player's movement.
 */
int getTemp() {
    int value1 = analogRead(THERMAL_PIN);
    delay(1);
    int value2 = analogRead(THERMAL_PIN);
    return (value1 + value2)/2;
}

/*
 * Plays the beginning of the music and displays "BreadMan"
 */
void titleScreen() {
    // There are a couple tricky things done here. First, this section of the 
    // music has a sequence of four notes repeated 8 times, and the title of 
    // the game has 8 characters, so they can go in time. I may or may not
    // have chosen the title with this in mind.
    for(int i = 0; i < 8; i++) {
        // Second, since we want to wait in between each of the notes, we start
        // playing the note, with the duration of a SIXTEENTH note, and then
        // While it plays, we display the current letter for the duration of a 
        // SIXTEENTH note. Even though we call displayLetter four times to show
        // one letter, it's okay since tone() is fast enough that you can't see
        // the interruption. 
        // 
        // The format for tone is 
        //   tone(pin, frequency, duration)
        tone(AUDIO_PIN, NOTE_G5, SIXTEENTH);
        displayLetter(BreadMan[i], SIXTEENTH);
        tone(AUDIO_PIN, NOTE_FS5, SIXTEENTH);
        displayLetter(BreadMan[i], SIXTEENTH);
        tone(AUDIO_PIN, NOTE_F5, SIXTEENTH);
        displayLetter(BreadMan[i], SIXTEENTH);
        tone(AUDIO_PIN, NOTE_G6, SIXTEENTH);
        displayLetter(BreadMan[i], SIXTEENTH);
        allOff();
    }
}

/*
 * Displays the letter specified by an 8x8 grid of 0s and 1s for ms many
 * milliseconds.
 */
void displayLetter(const int letter[8][8], unsigned long ms) {
    // The outer loop just keeps displaying the letter until time is up.
    // Note that, unlike other functions which we use to display information,
    // this one doesn't call allOff(), and instead sets individual columns off.
    // This is because we really care about time in the code that calls this
    // (titleScreen) because it's synced with the music, so we don't want to
    // add anything extra.
    unsigned long start = millis();
    while(millis() - start <= ms) {
        // On the inside, we just loop over the rows, set up the columns for 
        // each row, and then turn the row on until it's the next row's turn.
        for(int row = 0; row < 8; row++) {
            for(int col = 0; col < 8; col++) {
                if(letter[row][col]) {
                    digitalWrite(ledcol[col], HIGH);
                } else {
                    digitalWrite(ledcol[col], LOW);
                }
            }
            digitalWrite(ledrow[row], LOW);
            delay(1); // Leave the lights on briefly
            digitalWrite(ledrow[row], HIGH);
        }
    }
    // We have to turn the last row off, in case (or because) the calling code
    // doesn't clear the display.
    digitalWrite(ledrow[7], HIGH);
}


/* 
 * Main routine (called repeated by from the Arduino framework)
 * Repeatedly updates the player's position and the rest of the game state; this
 * is the central part of the control flow.
 */
void loop() {
    // Check the thermister to update the position.
    int position = getPosition();
    updateGameState();
    
    // Display the locations of everything for a fixed amount of time.
    // This also accesses the global obstacles variable.
    setDisplay(DISPLAY_INTERVAL, position);

    // If there isn't a collision, nothing else has to happen
    if(detectCollision(position)) {
        // They lost a life! Make noise and sound
        collisionAlert();
        lives--;

        if(lives == 0) {
            // Play the game-over animation
            gameOverAnim();

            // Show the score. The first 8 turns don't count, since the first
            // obstacle doesn't clear the screen until the 9th turn. And every
            // four turns translate to one obstacle, so divide by 4. But this 
            // truncates, so we're short by one. And then we don't want to count
            // the ones that they accidentally hit, so subtract STARTING_LIVES
            int score = (turnNumber - 8)/4 + 1 - STARTING_LIVES;
            displayScore(score);

            // Pause briefly, then restart the game.
            delay(CLOCK_INTERVAL);
            restart();
        }
    }
}

/*
 * Checks the temperature and computes the player's new location. If the 
 * location goes out of the screen, move the screen (but not the obstacles), to
 * accomodate.
 */
int getPosition() {
    int temperature = getTemp();
    // The basepoint represents the top of the screen - the top-left pixel is 
    // (0, 0).
    if(temperature < thermBasepoint) {
        // The player is above the screen, move the screen so they are at the 
        // top.
        thermBasepoint = temperature;
    } else if(temperature > thermBasepoint + 7) {
        // The player is below the bottom of the screen, move the screen so
        // they are at the bottom.
        thermBasepoint = temperature - 7;
    }
    return temperature - thermBasepoint;
}

/*
 * Waits for the next clock tick, and then updates the obstacles, the score, the
 * music, and the difficulty.
 */
void updateGameState() {
    // If the next clock tick has occured, update everything. Otherwise, wait
    // until next time.
    unsigned long now = millis();
    if(now - lastClock > CLOCK_INTERVAL) {
        // Update the time
        lastClock = now;
        turnNumber++;

        // Update difficulty. If they've been through the whole song, switch
        // to hard mode (twice as many obstacles).
        if(turnNumber > NUM_NOTES) {
            hardMode = 1;
        }

        // Play the next note in the song.
        updateMusic();

        // Shift all the obstacles forward.
        for(int row = 0; row < 7; row++) {
            obstacles[row] = obstacles[row+1];
        }

        // Get new obstacles for the last column.
        obstacles[7] = newCol();
    }
}

/* 
 * Plays the next note in the song. This accesses the global turnNumber
 * variable.
 */
void updateMusic() {
    // The music repeats every NUM_NOTES notes.
    int musicIndex = turnNumber % NUM_NOTES;
    int note = MUSIC[musicIndex];

    // Notes with 0 frequency represent rests, so ignore them.
    if(note != 0) {
        // Determine how long this note is held, and play it for that length of
        // time.
        int i = musicIndex;
        while(MUSIC[i] == note) {
            i++;
        }
        tone(AUDIO_PIN, note, CLOCK_INTERVAL*(i-musicIndex));
    }
}

/*
 * Pseudorandomly generate an integer representing a the obstacles in a new 
 * column. Only the last 8 bits of each element are used. If the ith smallest 
 * bit is set, then the ith row of the column has an obstacle.
 
 * The randomn(n) function here returns a pseudorandom integer from 0 to n-1.
 * This number is based on the seed passed to randomSeed() earlier (in setup()
 * or restart()).
 */
int newCol() {
    // Only put obstacles in one of every few columns.
    if(turnNumber % OBSTACLE_DISTANCE == 0) {
        if(hardMode) {
            // It's hardmode; make two obstacles. Get one even position and
            // one odd position.
            int oddObstacle = 2*random(4) + 1;
            int evenObstacle = 2*random(4);
            
            // Convert these two positions to our obstacle format.
            return (1 << oddObstacle) + (1 << evenObstacle);
        } else {
            // Generate a position for the obstacle
            int obstacle = random(8);

            // Convert the position into the obstacle format.
            return 1 << obstacle;
        }
    } else {
        return 0;
    }
}

/* 
 * Displays the normal part of the game, that is, the positions of the player 
 * and the obstacles, for ms many milliseconds. The player's position is passed
 * as the argument position, while the obstacles' positions are accessed via 
 * the global obstacles array.
 * 
 * This also makes the player's position brighter than the rest of the pixels.
 */
void setDisplay(int ms, int position) {
    // This is almost identical to the displayLetter() function, so see that one
    // for a more detailed explanation.
    unsigned long start = millis();
    int rowMask;
    while(millis() - start <= ms) {
        for(int row = 0; row < 8; row++) {
            // Make a bitmask to extract the bit for this row from each integer 
            // in obstacles.
            rowMask = 1 << row;

            digitalWrite(ledrow[prevrow], HIGH);

            // There are two things to display in the first column
            if((obstacles[0] & rowMask) || (position == row)) {
                digitalWrite(ledcol[0], HIGH);
            } else {
                digitalWrite(ledcol[0], LOW);
            }
            digitalWrite(ledrow[row], HIGH);

            for(int col = 1; col < 8; col++) {
                if(obstacles[col] & rowMask) {
                    digitalWrite(ledcol[col], HIGH);
                } else {
                    digitalWrite(ledcol[col], LOW);
                }
            }
            digitalWrite(ledrow[row], LOW);
            prevrow = row;
        }
    }
    digitalWrite(ledrow[prevrow], HIGH);

    // To make the player's position bright, we leave that pixel turned on when
    // we exit, so that it stays on until something else happens.
    digitalWrite(ledcol[0], HIGH);
    for(int col = 1; col < 8; col++) {
        digitalWrite(ledcol[col], LOW);
    }
    digitalWrite(ledrow[position], LOW);

    // This line is the reason that prevrow has to be global. Since we're 
    // leaving a light on when we exit, we have to make sure to turn it off at 
    // the start of the next time setDisplay is called.
    prevrow = position;
}

/* 
 * After the player's position and the blocks' positions have been updated, this
 * checks whether the new position collides with any obstacle.
 */
int detectCollision(int position) {
    // Create a bitmask to get the bit at index position. It is ensured by 
    // getPosition that 0 <= position < 8
    int mask = 1 << position;

    if(obstacles[0] & mask) {
        // There is an obstacle in this position - collision!
        return 1;
    } else {
        return 0;
    }
}

/* 
 * If the user collides with an obstacle, turn all the lights on briefly, play a
 * noise, and then resume the game.
 */
void collisionAlert() {
    // Play this tone until we call noTone()
    tone(AUDIO_PIN, NOTE_A2);

    // Blink the lights
    allOn();
    delay(CLOCK_INTERVAL);
    allOff();

    noTone(AUDIO_PIN);

    // Play this tone for CLOCK_INTERVAL many milliseconds.
    tone(AUDIO_PIN, NOTE_F2, CLOCK_INTERVAL);
}


/* 
 * Play an animation I made up when the player loses. 
 */
void gameOverAnim() {
    // Turn off the normal music and the display
    noTone(AUDIO_PIN);
    allOff();

    // Play the animation
    delay(2*CLOCK_INTERVAL);

    tone(AUDIO_PIN, NOTE_A3,  SIXTEENTH);
    delay(SIXTEENTH);
    tone(AUDIO_PIN, NOTE_GS3, SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[4], LOW);
    tone(AUDIO_PIN, NOTE_FS3, SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[3], HIGH);

    tone(AUDIO_PIN, NOTE_GS3, SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[3], LOW);
    tone(AUDIO_PIN, NOTE_FS3, SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[4], HIGH);
    tone(AUDIO_PIN, NOTE_E3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[5], LOW);

    tone(AUDIO_PIN, NOTE_FS3, SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[2], HIGH);
    tone(AUDIO_PIN, NOTE_E3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[2], LOW);
    tone(AUDIO_PIN, NOTE_D3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[5], HIGH);

    tone(AUDIO_PIN, NOTE_E3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[6], LOW);
    tone(AUDIO_PIN, NOTE_D3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[1], HIGH);
    tone(AUDIO_PIN, NOTE_CS3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[1], LOW);

    tone(AUDIO_PIN, NOTE_D3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[6], HIGH);
    tone(AUDIO_PIN, NOTE_CS3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[7], LOW);
    tone(AUDIO_PIN, NOTE_B2,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[0], HIGH);

    tone(AUDIO_PIN, NOTE_CS3,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledrow[0], LOW);
    tone(AUDIO_PIN, NOTE_B2,  SIXTEENTH);
    delay(SIXTEENTH);
    digitalWrite(ledcol[7], HIGH);
    tone(AUDIO_PIN, NOTE_A2, 4*CLOCK_INTERVAL);
    delay(4*CLOCK_INTERVAL);
}

/*
 * Displays the users score with an animation. Counts up to their score from 1
 * and plays a little chime with every other number.
 */
void displayScore(int score) {
    // Loop through the smaller numbers
    for(int i = 1; i < score; i++) {
        // On even ones, play one half of the chime, on odd ones play the other
        // half.
        if(i % 2 == 0) {
            tone(AUDIO_PIN, NOTE_C4, SCORE_ANIMATION_INTERVAL);
        } else {
            tone(AUDIO_PIN, NOTE_C5, SCORE_ANIMATION_INTERVAL);
        }
        // And display the number.
        displayNumber(i, SCORE_ANIMATION_INTERVAL);
    }

    // Finally, show the final score.
    displayNumber(score, SCORE_DISPLAY_TIME);
}

/*
 * Displays the decimal representation of an integer from 0 to 99 for a given 
 * length of time. Numbers above the range are represented as 99, and behavior 
 * is undefined for negative numbers.
 */
void displayNumber(int n, unsigned long ms) {
    // Parse the number in decimal
    if(n > 99) n = 99;
    int lowDigit = n % 10;
    int highDigit = n / 10;

    // Just like the other display functions, loops through rows and sets the 
    // appropriate columns in each, repeating until time is up.
    unsigned long start = millis();
    while(millis() - start <= ms) {
        for(int digitRow = 0; digitRow < 5; digitRow++) {
            for(int digitCol = 0; digitCol < 3; digitCol++) {
                if(highDigit > 0) {
                    if(DIGITS[highDigit][digitRow][digitCol]) {
                        digitalWrite(ledcol[digitCol], HIGH);
                    } else {
                        digitalWrite(ledcol[digitCol], LOW);
                    }
                }

                if(DIGITS[lowDigit][digitRow][digitCol]) {
                    digitalWrite(ledcol[digitCol+4], HIGH);
                } else {
                    digitalWrite(ledcol[digitCol+4], LOW);
                }
            }
            digitalWrite(ledrow[digitRow + 2], LOW);
            delay(1); // Leave it on briefly
            digitalWrite(ledrow[digitRow + 2], HIGH);
        }
    }

    // Leave the display the way we found it.
    allOff();
}

/*
 * The game ended. Reset the game state. Reseeds the PRNG in case the first 
 * seed had been seen before, resets the in-game statistics, and clears the 
 * obstacles. Finally, loads the title screen.
 */
void restart() {
    // Reseed the PRNG
    totalLifetimeTurns += turnNumber;
    int seed = totalLifetimeTurns + analogRead(THERMAL_PIN);
    randomSeed(seed);

    // Reset the stats
    turnNumber = 0;
    lives = STARTING_LIVES;
    hardMode = 0;

    // Sets the array to 0, removing all obstacles.
    memset(obstacles, 0, sizeof(obstacles)); 

    // Load the title screen before beginning again.
    titleScreen();
}

