#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// ====================================
// Constants and Configurations
// ====================================
#define MAX_QUESTIONS 100  // Fixed number of questions in the question bank
#define TEST_DURATION 360  // Test duration in seconds (6 minutes)


// ====================================
// Data Structures
// ====================================

// Structure to store question data including text, options and correct answer
typedef struct {
    char question[200];         // The question text
    char options[4][100];       // Array of four possible answer options
    int correctOption;          // Index of the correct option (1-4)
} Question;


// ====================================
// Function Declarations
// ====================================

// User Interface Functions
void greet();                                                  // Displays welcome message and game title
int getCategory();                                            // Gets user's preferred category (IQ Test/Math Quiz/English Quiz)
int getGameMode();                                            // Gets user's preferred game mode (Solo/1v1/Multiplayer)
void getPlayerNames(char player1[], char player2[], int mode); // Gets names of participating players
void getMultiplePlayerNames(char names[][50], int numPlayers); // Gets names for multiple players
void playMultiplayer(int numQuestions, Question questions[], int totalQuestions, char names[][50], int numPlayers, int category); // Handles multiplayer gameplay

// Game Logic Functions
void loadQuestions(Question questions[], int *totalQuestions, int category); // Loads question bank into memory
int playSolo(int numQuestions, Question questions[], int totalQuestions, char playerName[], int *timeTaken, int category); // Handles solo gameplay
void play1v1(int numQuestions, Question questions[], int totalQuestions, char player1[], char player2[], int category); // Manages 1v1 gameplay
int askQuestion(Question q);                    // Presents question and validates answer

// Results and Scoring Functions
void declareWinner(char player1[], char player2[], int score1, int score2, int timeTaken1, int timeTaken2); // Determines winner in 1v1 mode
void displayIQScore(char playerName[], int score, int totalQuestions); // Calculates and shows IQ score


// ====================================
// Main Program Entry Point
// ====================================
int main() {
    // Initialize random number generator for question shuffling
    srand(time(0)); 

    // Display welcome screen
    greet(); 

    // Get game configuration
    int category = getCategory();
    int mode = getGameMode();
    int quizSize = 10;             // Set fixed quiz length

    // Initialize question bank
    Question questions[MAX_QUESTIONS];
    int totalQuestions = 0;
    loadQuestions(questions, &totalQuestions, category);

    // Handle different game modes
    if (mode == 1) { // Solo Play
        char player1[50];
        printf("\nEnter your name: ");
        scanf(" %[^\n]", player1);
        int timeTaken;
        playSolo(quizSize, questions, totalQuestions, player1, &timeTaken, category);
    } 
    else if (mode == 2) { // Challenge a Friend
        char player1[50], player2[50];
        printf("\nEnter Player 1's name: ");
        scanf(" %[^\n]", player1);
        printf("Enter Player 2's name: ");
        scanf(" %[^\n]", player2);
        play1v1(quizSize, questions, totalQuestions, player1, player2, category);
    }
    else { // Challenge Multiple Friends
        int numPlayers;
        printf("\nHow many players will participate? ");
        scanf("%d", &numPlayers);
        
        // Validate number of players
        if (numPlayers < 2) {
            printf("Invalid number of players. Defaulting to 2 players.\n");
            numPlayers = 2;
        }
        
        char playerNames[numPlayers][50];
        getMultiplePlayerNames(playerNames, numPlayers);
        playMultiplayer(quizSize, questions, totalQuestions, playerNames, numPlayers, category);
    }

    return 0;
}


// ====================================
// Function Implementations - User Interface
// ====================================

// Displays welcoming message with decorative formatting
void greet() {
    printf("\n====================================\n");
    printf("   Welcome to MindMatch !\n");
    printf("====================================\n\n");
}

// Gets and validates user's category selection
int getCategory() {
    int category;

    printf("Select Category:\n");
    printf("1 - IQ Test\n");
    printf("2 - Math Quiz\n");
    printf("3 - English Quiz\n");
    printf("Enter your choice: ");
    scanf("%d", &category);

    // Input validation - defaults to IQ if invalid
    return (category >= 1 && category <= 3) ? category : 1;
}

// Gets and validates user's game mode selection
int getGameMode() {
    int mode;

    printf("\nSelect Game Mode:\n");
    printf("1 - Solo Play\n");
    printf("2 - Challenge a Friend\n");
    printf("3 - Challenge Multiple Friends\n");
    printf("Enter your choice: ");
    scanf("%d", &mode);

    // Input validation - defaults to Solo mode if invalid
    return (mode >= 1 && mode <= 3) ? mode : 1;
}

// Collects player names with input validation
void getPlayerNames(char player1[], char player2[], int mode) {
    if (mode == 1) {
        printf("\nEnter your name: ");
        scanf(" %[^\n]", player1);
    } else {
        printf("\nEnter Player 1's name: ");
        scanf(" %[^\n]", player1);
        printf("Enter Player 2's name: ");
        scanf(" %[^\n]", player2);
    }
}

// Gets names for multiple players
void getMultiplePlayerNames(char names[][50], int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        printf("Enter Player %d's name: ", i + 1);
        scanf(" %[^\n]", names[i]);
    }
}


void loadQuestions(Question questions[], int *totalQuestions, int category) {
    // First, load all questions into the array
    *totalQuestions = 100;
    
    // ====================================
    // Vocabulary & Word Meaning Questions
    // ====================================
    Question q1 = {"Which word does not have a similar meaning to 'imaginary'?", {"inconsistent", "fictional", "illusive", "apocryphal"}, 1};
    Question q2 = {"Which word does not have a similar meaning to 'fatigued'?", {"enervated", "strength", "tired","exhaustion" }, 2};
    Question q3 = {"Which word does not have a similar meaning to 'beautiful'?", {"elegant", "stunning","grotesque" , "exquisite"}, 3};
    Question q4 = {"Find two words, one from each group, that are closest in meaning: Group A: Confused, enraged, terrified Group B: Calm, trance, incensed",
     {"confused and calm", "terrified and trance", "enraged and calm" ,"enraged and incensed"}, 4};

    Question q5 = {"Which word means 'having been deserted or left'?", {"abandoned", "deluded", "truant", "mindful"}, 1};
    Question q6 = {"Which word can be used to describe the part of the body that contains all structures between the chest and pelvis?",
     { "pleural","abdominal", "articular", "wreathed"}, 2};
    Question q7 = {"Which word is most similar in meaning to 'betraying'?", {"faithful", "loyal","treacherous" , "steadfast"}, 3};
    Question q8 = {"Which word means 'a lack of generosity or willingness to share'?", {"altruistic", "bountiful", "charitable","miserly" }, 4};

    Question q9 = {"Which word means 'to damage or destroy secretly'?", {"sabotaged", "repaired", "enhanced", "constructed"}, 1};
    Question q10 = {"Which word means 'the act of thinking deeply or carefully about something'?", { "dismissal","deliberation", "ignorance", "neglect"}, 2};
    Question q11 = {"Which word means 'the act of giving up or surrendering something'?", {"persistence", "resistance","abandonment" , "defiance"}, 3};
    Question q12 = {"Which word does not have a similar meaning to 'honorable'?", {"ethical", "principled", "moral", "deceitful"}, 4};

    Question q13 = {"What is the opposite of 'believable'?", {"incredulous", "plausible", "credible", "trustworthy"}, 1};
    Question q14 = {"Which word means 'a person who gives disapproval'?", {"admirer", "critic", "supporter", "advocate"}, 2};
    Question q15 = {"Which word does not belong with the others?", {"spaghetti", "macaroni", "pancake", "pasta"}, 3};
    Question q16 = {"Which word means 'acceptable, fair, or good enough'?", {"unacceptable", "inadequate", "insufficient", "satisfactory"}, 4};

    Question q17 = {"What is the opposite of 'respectful'?", {"disrespectful", "considerate", "polite", "courteous"}, 1};
    Question q18 = {"Which word means 'a sense of pride or pleasure'?", {"disappointment", "satisfaction", "frustration", "dissatisfaction"}, 2};
    Question q19 = {"Which word does not mean 'something that can be questioned'?", {"disputable", "debatable", "certainty", "questionable"}, 3};
    Question q20 = {"Which word means 'ethical, fair, and principled'?", {"dishonest", "corrupt", "unethical", "honourable"}, 4};

    Question q21 = {"Which word means 'the process of carefully considering something before making a decision'?",
     {"contemplation", "impulsiveness", "recklessness", "haste"}, 1};
    Question q22 = {"Which word means 'the act of giving freely without expecting anything in return'?", {"stinginess", "generosity", "selfishness", "greed"}, 2};
    Question q23 = {"Which word does not have a similar meaning to 'belittle'?", {"criticise", "downgrade", "overrate", "discredit"}, 3};
    Question q24 = {"Which word means 'a sense of pride or pleasure derived from achievement'?", {"disappointment", "frustration", "dissatisfaction", "satisfaction"}, 4};
    
    Question q25 = {"Which word does not have a similar meaning to 'important'?", {"miniature", "significant", "imperative", "of substance"}, 1};

    // ====================================
    // Grammar Questions
    // ====================================
    Question q26 = {"Which sentence is grammatically correct?", 
     {"We regret to be in contact with you shortly.", "We will be in contact with you shortly.",
      "Shortly, we will be in contract with you.", "Will be in contract with you shortley."}, 2};
    Question q27 = {"Which sentence is grammatically correct?", 
     {"I wishes you the very best of luck.", "Our holiday as been postponed.", 
     "It is difficult to understand my teacher.", "The Government are implementing a new law today."}, 3};
    Question q28 = {"Which sentence is grammatically correct?", 
    {"Every days he bring me flowers.","Every days he brings me flower.",
     "Every day he bringing me flowers.",  "Every day he brings me flowers."}, 4};

    Question q29 = {"Choose the grammatically correct option:", 
    {"Prior to the airplane taking off, the pilot informs about safety procedures.", "The pilot the helicopter back to basecamp due terrential winds.",
     "Prior to the airplane taking off, the informs about safety.", "The pilot navigate the plane through stormy weather."}, 1};
    Question q30 = {"Which sentence is grammatically correct?", 
    {"The firefighter service are more than just putting out fires.", "The firefighter service is more than just putting out fires.", 
    "Firefighters service is more than just putting out fire.", "The firefighters service is more than just putting out fires."}, 2};
    Question q31 = {"Which sentence is grammatically correct?", 
     {"The queen are bout to give a speech.", "The queen was about too give a speech.", "The queen is about to give a speech.", "The queen will giving a speech."}, 3};
    Question q32 = {"Identify the grammatically correct sentence:", 
    {"Every day he bring me flowers.", "Every days he brings me flower.", "Every day he bringing me flowers.", "Every day he brings me flowers."}, 4};

    Question q33 = {"Which combination of words can be inserted into the sentence? 'We regret to inform you that your application was ____________.'", 
     {"unsuccessful", "success", "succeeding", "succeed"}, 1};
    Question q34 = {"Which sentence is grammatically correct?", 
     {"The fire departments provides emergency services.", "The fire department provides emergency services.", 
     "Fire departments provide emergency services.", "The fire department are providing emergency services."}, 2};
    Question q35 = {"Which sentence is grammatically correct?", 
     {"The police has new suspect.", "The police as a new lead.", "The police have a new suspect.", "The police not have any leads to go on."}, 3};
    Question q36 = {"Insert the correct combination of words into the sentence: 'Law is the system of rules, guidelines, and conducts established by the ______ government that maintains a safe and stable society.'",
     {"generous", "friendly", "vulgar", "sovereign"}, 4};

    Question q37 = {"Which combination of words completes the sentence correctly? 'A lawyer is a person that is learned in law; whether that be as an ______, counsel, or solicitor.'",
     {"attorney", "generous", "friendly", "vulgar"}, 1};
    Question q38 = {"Which combination of words can be inserted into the sentence? 'We are pleased to inform you that your application has been ____________.'",
     {"rejected", "accepted", "pending", "reviewed"}, 2};
    Question q39 = {"Which sentence is grammatically correct?",
     {"She always bring me coffee in the morning.", "She always bringing me coffee in the morning.",
      "She always brings me coffee in the morning.", "She always brings me coffee in the mornings."}, 3};
    Question q40 = {"Which sentence is grammatically correct?", 
     {"The police departments maintains public safety.", "Police departments maintain public safety.",
      "The police department are maintaining public safety.", "The police department maintains public safety."}, 4};

    // ====================================
    // Number Sequence Questions
    // ====================================
    Question q41 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 4, 9, 16, 25, 36, 49", {"64, 81", "63, 74", "51, 80", "54, 63"}, 1};
    Question q42 = {"What comes next in this sequence? 0, 1, 1, 2, 3, 5, 8", {"12, 18", "13, 21", "15, 25", "15, 23"}, 2};
    Question q43 = {"What is the next pair in the sequence? 27, 25, 87, 23, 21, 87, 19", {"16, 87", "87, 17" , "17, 87", "16, 13"}, 3};
    Question q44 = {"Complete the sequence: 1, 3, 7, 15, 31, 63, 127", {"220, 440", "136, 210", "187, 316", "255, 511"}, 4};

    Question q45 = {"What comes next? 1, 3, 6, 10, 15, 21, 28", {"36, 45", "42, 56", "42, 48", "30, 36"}, 1};
    Question q46 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 2, 4, 8, 16, 32, 64", {"120, 240", "128, 256", "130, 260", "140, 280"}, 2};
    Question q47 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 4, 9, 16, 25, 36", {"45, 60", "50, 65", "49, 64", "55, 70"}, 3};
    Question q48 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 3, 6, 10, 15, 21", {"25, 30", "30, 40", "35, 45", "28, 36"}, 4};

    Question q49 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 2, 5, 10, 17, 26, 37", {"50, 65", "45, 60", "55, 70", "60, 75"}, 1};
    Question q50 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 2, 4, 7, 11, 16", {"20, 25", "22, 29", "25, 30", "30, 35"}, 2};
    Question q51 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 3, 6, 12, 24, 48, 96", {"180, 360", "200, 400", "192, 384", "220, 440"}, 3};
    Question q52 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 5, 13, 29, 61, 125", {"250, 500", "260, 520", "270, 540", "253, 509"}, 4};

    Question q53 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 4, 9, 16, 25, 36", {"49, 64", "45, 60", "50, 65", "55, 70"}, 1};
    Question q54 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 2, 6, 12, 20, 30, 42", {"50, 60", "56, 72", "60, 70", "70, 80"}, 2};
    Question q55 = {"Look carefully for the pattern, and then choose which pair of numbers comes next: 1, 3, 7, 15, 31, 63", {"120, 240", "130, 260", "127, 255", "140, 280"}, 3};

    // ====================================
    // Ratio Questions
    // ====================================
  Question q56 = {"Peter has £15.20, and Jim has £20.80. What is the ratio of Peter's money to Jim's money in its simplest form?", {"15:20", "38:52", "10:13", "19:26"}, 4};

Question q57 = {"A newspaper includes 16 pages of sport and 8 pages of TV. What is the ratio of sport to TV?", {"2:1", "1:2", "4:1", "3:1"}, 1};  
Question q58 = {"A magazine includes 24 pages of fashion and 8 pages of lifestyle. What is the ratio of fashion to lifestyle?", {"2:1", "3:1", "1:3", "4:1"}, 2};  
Question q59 = {"A book contains 240 pages for its novel and 24 pages for introductions and summaries. What is the ratio of novel pages to introductions and summaries?", 
{"5:1", "20:1", "10:1", "1:10"}, 3}; 
Question q60 = {"A school has to mark 180 English papers and 170 Maths papers. What is the ratio of English papers to Maths papers?", {"17:18", "9:8", "10:9", "18:17"}, 4};

Question q61 = {"A class has 12 boys and 18 girls. What is the ratio of boys to girls in its simplest form?", {"2:3", "3:2", "4:6", "6:9"}, 1};  
Question q62 = {"A bag contains 8 red marbles and 12 blue marbles. What is the ratio of red to blue marbles in its simplest form?", {"2:3", "3:2", "4:6", "6:9"}, 2};  
Question q63 = {"A recipe uses 2 cups of flour and 3 cups of sugar. What is the ratio of flour to sugar in its simplest form?", {"4:6", "3:2", "2:3", "6:9"}, 3}; 
Question q64 = {"A garden has 15 roses and 25 tulips. What is the ratio of roses to tulips in its simplest form?", {"9:15", "5:3", "6:10", "3:5"}, 4}; 

Question q65 = {"A box contains 20 apples and 30 oranges. What is the ratio of apples to oranges in its simplest form?", {"2:3", "3:2", "4:6", "6:9"}, 1};  
Question q66 = {"A school has 24 teachers and 36 students. What is the ratio of teachers to students in its simplest form?", {"2:3", "3:2", "4:6", "6:9"}, 2};  
Question q67 = {"A store has 16 shirts and 24 pants. What is the ratio of shirts to pants in its simplest form?", {"4:6", "3:2", "2:3", "6:9"}, 3}; 
Question q68 = {"A park has 18 trees and 27 bushes. What is the ratio of trees to bushes in its simplest form?", {"6:9", "3:2", "4:6", "2:3"}, 4};  

Question q69 = {"A zoo has 20 lions and 30 tigers. What is the ratio of lions to tigers in its simplest form?", {"2:3", "3:2", "4:6", "6:9"}, 1}; 
Question q70 = {"A farm has 14 cows and 21 sheep. What is the ratio of cows to sheep in its simplest form?", {"2:3", "3:2", "4:6", "6:9"}, 2};  
    // ====================================
    // General Knowledge Questions
    // ====================================
Question q71 = {"What is the capital of France?", {"Lyon", "Marseille", "Paris", "Bordeaux"}, 3};
Question q72 = {"Which planet is known as the Red Planet?", {"Venus", "Jupiter", "Saturn", "Mars"}, 4};

Question q73 = {"Who wrote 'Hamlet'?", {"William Shakespeare", "Charles Dickens", "Mark Twain", "Jane Austen"}, 1};
Question q74 = {"What is the chemical symbol for water?", {"CO₂", "H₂O", "O₂", "NaCl"}, 2};
Question q75 = {"Which element has the atomic number 1?", {"Helium", "Oxygen", "Hydrogen", "Carbon"}, 3};

    // ====================================
    // Mental Math Questions
    // ====================================
Question q76 = {"What is 8 multiplied by 6?", {"42", "54", "60", "48"}, 4};

Question q77 = {"What is 4 multiplied by 7?", {"28", "24", "32", "36"}, 1};
Question q78 = {"What is 6 multiplied by 9?", {"48", "54", "52", "60"}, 2};
Question q79 = {"What is 9 multiplied by 9?", {"72", "84", "81", "90"}, 3};
Question q80 = {"What is 8 multiplied by 7?", {"54", "63", "64", "56"}, 4};

Question q81 = {"What is 12 multiplied by 6?", {"72", "66", "60", "78"}, 1};
Question q82 = {"What is 13 multiplied by 7?", {"84", "91", "81", "96"}, 2};
Question q83 = {"What is 3/4 of 80 plus 1/5 of 75?", {"65", "85", "75", "95"}, 3};
Question q84 = {"Subtract 1/3 of 90 from 2/5 of 150.", {"20","50", "40",  "30"}, 4};

Question q85 = {"Multiply 1/2 of 48 by 3/4 of 20.", {"360", "420", "480", "540"}, 1};
Question q86 = {"What is 25% of 240 minus 15% of 120?", {"36",  "42","48", "54"}, 2};
Question q87 = {"Add 3/8 of 64 to 5/6 of 36.", {"48", "60", "54", "66"}, 3};
Question q88 = {"What is 7/10 of 120 minus 2/5 of 50?", {"56", "80", "72", "64"}, 4};

Question q89 = {"Divide 3/4 of 144 by 2/3 of 36.", {"5", "3", "4", "6"}, 1};
Question q90 = {"Multiply 5/6 of 72 by 2/3 of 45.", {"1350", "1800", "1500", "1650"}, 2};
Question q91 = {"What is 40% of 180 plus 60% of 120?", {"120", "168", "144", "192"}, 3};
Question q92 = {"Subtract 1/4 of 200 from 3/5 of 250.", {"80", "140", "120", "100"}, 4};

Question q93 = {"What is 5/8 of 160 plus 3/10 of 100?", {"130", "120", "140", "150"}, 1};
Question q94 = {"Add 2/3 of 90 to 4/5 of 75.", {"110", "120", "130", "140"}, 2};
Question q95 = {"Multiply 3/4 of 80 by 5/6 of 36.", {"600", "540", "1800", "660"}, 3};
Question q96 = {"What is 15% of 300 minus 10% of 150?", {"25", "30", "35", "40"}, 4};

Question q97 = {"Divide 7/10 of 200 by 1/2 of 40.", {"7", "6", "8", "9"}, 1};
Question q98 = {"What is 3/4 of 108 plus 2/5 of 65?", {"105", "100", "110", "107"}, 2};
Question q99 = {"Add 3/7 of 63 to 2/3 of 45.", {"60", "55", "65", "57"}, 3};
Question q100 = {"Subtract 1/4 of 140 from 2/3 of 450.", {"250", "270", "280", "265"}, 4};

    // Assign all questions to the array
    questions[0] = q1;
    questions[1] = q2;
    questions[2] = q3;
    questions[3] = q4;
    questions[4] = q5;
    questions[5] = q6;
    questions[6] = q7;
    questions[7] = q8;
    questions[8] = q9;
    questions[9] = q10;
    questions[10] = q11;
    questions[11] = q12;
    questions[12] = q13;
    questions[13] = q14;
    questions[14] = q15;
    questions[15] = q16;
    questions[16] = q17;
    questions[17] = q18;
    questions[18] = q19;
    questions[19] = q20;
    questions[20] = q21;
    questions[21] = q22;
    questions[22] = q23;
    questions[23] = q24;
    questions[24] = q25;
    questions[25] = q26;
    questions[26] = q27;
    questions[27] = q28;
    questions[28] = q29;
    questions[29] = q30;
    questions[30] = q31;
    questions[31] = q32;
    questions[32] = q33;
    questions[33] = q34;
    questions[34] = q35;
    questions[35] = q36;
    questions[36] = q37;
    questions[37] = q38;
    questions[38] = q39;
    questions[39] = q40;
    questions[40] = q41;
    questions[41] = q42;
    questions[42] = q43;
    questions[43] = q44;
    questions[44] = q45;
    questions[45] = q46;
    questions[46] = q47;
    questions[47] = q48;
    questions[48] = q49;
    questions[49] = q50;
    questions[50] = q51;
    questions[51] = q52;
    questions[52] = q53;
    questions[53] = q54;
    questions[54] = q55;
    questions[55] = q56;
    questions[56] = q57;
    questions[57] = q58;
    questions[58] = q59;
    questions[59] = q60;
    questions[60] = q61;
    questions[61] = q62;
    questions[62] = q63;
    questions[63] = q64;
    questions[64] = q65;
    questions[65] = q66;
    questions[66] = q67;
    questions[67] = q68;
    questions[68] = q69;
    questions[69] = q70;
    questions[70] = q71;
    questions[71] = q72;
    questions[72] = q73;
    questions[73] = q74;
    questions[74] = q75;
    questions[75] = q76;
    questions[76] = q77;
    questions[77] = q78;
    questions[78] = q79;
    questions[79] = q80;
    questions[80] = q81;
    questions[81] = q82;
    questions[82] = q83;
    questions[83] = q84;
    questions[84] = q85;
    questions[85] = q86;
    questions[86] = q87;
    questions[87] = q88;
    questions[88] = q89;
    questions[89] = q90;
    questions[90] = q91;
    questions[91] = q92;
    questions[92] = q93;
    questions[93] = q94;
    questions[94] = q95;
    questions[95] = q96;
    questions[96] = q97;
    questions[97] = q98;
    questions[98] = q99;
    questions[99] = q100;

    // Now, based on category, adjust which questions are available
    switch(category) {
        case 1: // IQ Test - uses all questions (1-100)
            *totalQuestions = 100;
            break;
            
        case 2: // Math Quiz - uses questions 56-70 and 76-100
            *totalQuestions = 40; // 15 from 56-70 + 25 from 76-100
            // Copy math questions to the beginning of the array
            for (int i = 0; i < 15; i++) {
                questions[i] = questions[55 + i]; // Questions 56-70
            }
            for (int i = 15; i < 40; i++) {
                questions[i] = questions[75 + (i-15)]; // Questions 76-100
            }
            break;
            
        case 3: // English Quiz - uses questions 1-40
            *totalQuestions = 40;
            // No need to copy, first 40 questions are already in place
            break;
    }
}


// ====================================
// Function Implementations - Game Logic
// ====================================

// Conducts a solo IQ test with timer functionality
int playSolo(int numQuestions, Question questions[], int totalQuestions, char playerName[], int *timeTaken, int category) {
    printf("\nStarting Solo Quiz...\n");
    printf("You have 6 minutes to complete the quiz!\n");

    int score = 0;
    time_t startTime = time(NULL);

    // Load category-specific questions
    Question categoryQuestions[MAX_QUESTIONS];
    int categoryTotalQuestions;
    loadQuestions(categoryQuestions, &categoryTotalQuestions, category);

    // Shuffle questions
    int indices[MAX_QUESTIONS];
    for (int i = 0; i < categoryTotalQuestions; i++) {
        indices[i] = i;
    }
    for (int i = categoryTotalQuestions - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Ask questions
    for (int i = 0; i < numQuestions; i++) {
        int randIndex = indices[i];
        printf("\n%s, it's your turn!\n", playerName);
        
        score += askQuestion(categoryQuestions[randIndex]);
        
        if (difftime(time(NULL), startTime) > TEST_DURATION) {
            printf("\nTime's up! The test has ended.\n");
            break;
        }

        int timeLeft = TEST_DURATION - difftime(time(NULL), startTime);
        int minutes = timeLeft / 60;
        int seconds = timeLeft % 60;
        printf("Time remaining: %02d:%02d\n", minutes, seconds);
    }

    *timeTaken = (int)difftime(time(NULL), startTime);

    // Display results based on category
    if (category == 1) {
        displayIQScore(playerName, score, numQuestions);
    } else {
        printf("\n%s, your Final Score: %d/%d\n", playerName, score, numQuestions);
    }

    return score;
}


// Manages the 1v1 gameplay mode between two players
void play1v1(int numQuestions, Question questions[], int totalQuestions, char player1[], char player2[], int category) {
    printf("\nStarting Multiplayer Quiz!\n");

    int timeTaken1, timeTaken2;

    printf("\n%s's turn!\n", player1);
    int score1 = playSolo(numQuestions, questions, totalQuestions, player1, &timeTaken1, category);

    printf("\n%s's turn!\n", player2);
    int score2 = playSolo(numQuestions, questions, totalQuestions, player2, &timeTaken2, category);

    // Display results based on category
    if (category == 1) {
        declareWinner(player1, player2, score1, score2, timeTaken1, timeTaken2);
    } else {
        printf("\n=================================\n");
        printf("        FINAL SCORES\n");
        printf("=================================\n");
        printf("%s: %d/%d\n", player1, score1, numQuestions);
        printf("%s: %d/%d\n", player2, score2, numQuestions);
        
        if (score1 > score2) {
            printf("%s wins!\n", player1);
        } else if (score2 > score1) {
            printf("%s wins!\n", player2);
        } else {
            printf("It's a tie!\n");
        }
    }
}


// Presents a question to the user and validates their answer
int askQuestion(Question q) {
    int answer;
    printf("\n%s\n", q.question);
    for (int i = 0; i < 4; i++) {
        printf("%d) %s\n", i + 1, q.options[i]);
    }
    printf("Enter your answer (1-4): ");
    scanf("%d", &answer);

    // Check if the answer is correct
    if (answer == q.correctOption) {
        printf("Correct!\n");
        return 1;
    } 
    else {
        printf("Wrong! The correct answer was: %s\n", q.options[q.correctOption - 1]);
        return 0;
    }
}


// ====================================
// Function Implementations - Results
// ====================================

// Determines and announces the winner in 1v1 mode
void declareWinner(char player1[], char player2[], int score1, int score2, int timeTaken1, int timeTaken2) {
    printf("\n=================================\n");
    printf("        FINAL SCORES\n");
    printf("=================================\n");
    printf("%s: %d (Time: %d seconds)\n", player1, score1, timeTaken1);
    printf("%s: %d (Time: %d seconds)\n", player2, score2, timeTaken2);

    // Determine and display the winner
    if (score1 > score2) {
        printf("%s is the smartest here !!!!!!!!!!!!\n", player1);
    } 
    else if (score2 > score1) {
        printf("%s is the smartest here !!!!!!!!!!!!\n", player2);
    } 
    else {
        // If scores are tied, check time taken
        if (timeTaken1 < timeTaken2) {
            printf("%s wins by time taken!\n", player1);
        } 
        else if (timeTaken2 < timeTaken1) {
            printf("%s wins by time taken!\n", player2);
        } 
        else {
            printf("U R both smart it is a tie.\n");
        }
    }
}


// Calculates and displays the player's IQ score and category
void displayIQScore(char playerName[], int score, int totalQuestions) {
    int iqMin, iqMax;
    char category[50];

    // Map score to IQ range and category with more granular ranges
    switch(score) {
        case 10:
            iqMin = 140;
            iqMax = 160;
            sprintf(category, "Exceptional Genius!");
            break;
        case 9:
            iqMin = 130;
            iqMax = 140;
            sprintf(category, "Very Superior Intelligence!");
            break;
        case 8:
            iqMin = 120;
            iqMax = 130;
            sprintf(category, "Superior Intelligence!");
            break;
        case 7:
            iqMin = 110;
            iqMax = 120;
            sprintf(category, "High Average Intelligence!");
            break;
        case 6:
            iqMin = 100;
            iqMax = 110;
            sprintf(category, "Average Intelligence!");
            break;
        case 5:
            iqMin = 90;
            iqMax = 100;
            sprintf(category, "Low Average Intelligence!");
            break;
        case 4:
            iqMin = 80;
            iqMax = 90;
            sprintf(category, "Below Average Intelligence!");
            break;
        case 3:
            iqMin = 70;
            iqMax = 80;
            sprintf(category, "Borderline Intelligence!");
            break;
        case 2:
            iqMin = 60;
            iqMax = 70;
            sprintf(category, "Mild Intellectual Disability!");
            break;
        case 1:
            iqMin = 50;
            iqMax = 60;
            sprintf(category, "Moderate Intellectual Disability!");
            break;
        case 0:
            iqMin = 40;
            iqMax = 50;
            sprintf(category, "Severe Intellectual Disability!");
            break;
        default:
            iqMin = 0;
            iqMax = 0;
            sprintf(category, "Invalid Score!");
    }

    // Display the final result
    printf("\n%s, your Final Score: %d/%d | IQ Range: %d-%d | Category: %s\n",
           playerName, score, totalQuestions, iqMin, iqMax, category);
}

// Handles multiplayer gameplay with unlimited players
void playMultiplayer(int numQuestions, Question questions[], int totalQuestions, char names[][50], int numPlayers, int category) {
    printf("\nStarting Multiplayer Quiz!\n");
    
    // Validate totalQuestions
    if (totalQuestions < numQuestions) {
        printf("Warning: Not enough questions available. Using all available questions.\n");
        numQuestions = totalQuestions;
    }
    
    // Shuffle questions for each player
    int indices[MAX_QUESTIONS];
    for (int i = 0; i < totalQuestions; i++) {
        indices[i] = i;
    }
    for (int i = totalQuestions - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    
    int scores[MAX_QUESTIONS] = {0};
    int timeTaken[MAX_QUESTIONS] = {0};
    
    // Each player takes their turn
    for (int i = 0; i < numPlayers; i++) {
        printf("\n%s's turn!\n", names[i]);
        // Use the shuffled questions
        Question shuffledQuestions[MAX_QUESTIONS];
        for (int j = 0; j < numQuestions; j++) {
            shuffledQuestions[j] = questions[indices[j]];
        }
        scores[i] = playSolo(numQuestions, shuffledQuestions, numQuestions, names[i], &timeTaken[i], category);
    }
    
    // Display results
    printf("\n=================================\n");
    printf("        FINAL SCORES\n");
    printf("=================================\n");
    
    // Display scores based on category
    for (int i = 0; i < numPlayers; i++) {
        if (category == 1) {
            printf("%s: %d (Time: %d seconds)\n", names[i], scores[i], timeTaken[i]);
        } else {
            printf("%s: %d/%d\n", names[i], scores[i], numQuestions);
        }
    }
    
    // Find winner(s)
    int maxScore = scores[0];
    int fastestTime = timeTaken[0];
    int winnerIndex = 0;
    int isTie = 0;
    
    for (int i = 1; i < numPlayers; i++) {
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            winnerIndex = i;
            fastestTime = timeTaken[i];
            isTie = 0;
        } else if (scores[i] == maxScore) {
            // In case of tie, use time as tiebreaker
            if (timeTaken[i] < fastestTime) {
                fastestTime = timeTaken[i];
                winnerIndex = i;
            }
            isTie = 1;
        }
    }
    
    // Handle ties
    if (isTie) {
        printf("\nIt's a tie between: ");
        for (int i = 0; i < numPlayers; i++) {
            if (scores[i] == maxScore) {
                printf("%s ", names[i]);
            }
        }
        printf("\n");
    } else {
        printf("\n%s wins!\n", names[winnerIndex]);
    }
}