#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/* Very slow seed: 686846853 */

#include "dungeon.h"
#include "pc.h"
#include "npc.h"
#include "move.h"
#include "io.h"

const char *victory =
  "\n                                       o\n"
  "                                      $\"\"$o\n"
  "                                     $\"  $$\n"
  "                                      $$$$\n"
  "                                      o \"$o\n"
  "                                     o\"  \"$\n"
  "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
  "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
  "\"oo   o o o o\n"
  "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
  "   \"o$$\"    o$$\n"
  "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
  "     o\"\"\n"
  "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
  "   o\"\n"
  "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
  "\"$$$  $\n"
  "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
  "\"\"      \"\"\" \"\n"
  "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
  "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
  "\"$$$$\n"
  "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
  "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
  "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
  "              $\"                                                 \"$\n"
  "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
  "$\"$\"$\"$\"$\"$\"$\"$\n"
  "                                   You win!\n\n";

const char *tombstone =
  "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
  "               /     /         \\             __\n"
  "              /     /           \\            ||\n"
  "             /____ /   Rest in   \\           ||\n"
  "            |     |    Pieces     |          ||\n"
  "            |     |               |          ||\n"
  "            |     |   A. Luser    |          ||\n"
  "            |     |               |          ||\n"
  "            |     |     * *   * * |         _||_\n"
  "            |     |     *\\/* *\\/* |        | TT |\n"
  "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
  "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
  "            |     |         \\/..\"\"\"\"\"...\"\"\""
  "\\ || /.\"\"\".......\"\"\"\"...\n"
  "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
  "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
  "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
  "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
  "            You're dead.  Better luck in the next life.\n\n\n";

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n|--nummon <count>]",
          name);

  exit(-1);
}
enum color{RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, WHITE, BLACK};

class dice{
public:
  int base;
  int dice;
  int sides;

  void print_dice(){
    cout << base <<'+'<<dice<<'d'<<sides;
  }

  void clear(){
    base = dice = sides = 0;
  }
};

class monster{
public:
  std::string name;
  std::string desc;
  uint8_t color;
  dice speed;
  uint8_t abil;
  std::string abil_string;
  dice hp;
  dice dam;
  char symb;
  int rrty;

  int is_complete(){
    return 1;
  }

  void clear_atts(){
    name = "";
    desc = "";
    color = 0;
    speed.clear();
    abil = 0;
    abil_string = "";
    hp.clear();
    dam.clear();
    symb = ' ';
    rrty = 0;
  }

  void add_speed(std::string di_str){
    dice di;
    std::string plus = "+";
    std::string d_d = "d";
    di.base = atoi(di_str.substr(0, di_str.find(plus)).c_str());
    di.dice = atoi(di_str.substr(di_str.find(plus)+1, di_str.find(d_d)).c_str());
    di.sides = atoi(di_str.substr(di_str.find(d_d)+1, di_str.length()-1).c_str());
    speed = di;
  }

  void add_dam(std::string di_str){
    dice di;
    std::string plus = "+";
    std::string d_d = "d";
    di.base = atoi(di_str.substr(0, di_str.find(plus)).c_str());
    di.dice = atoi(di_str.substr(di_str.find(plus)+1, di_str.find(d_d)).c_str());
    di.sides = atoi(di_str.substr(di_str.find(d_d)+1, di_str.length()-1).c_str());
    dam = di;
  }

  void add_hp(std::string di_str){
    dice di;
    std::string plus = "+";
    std::string d_d = "d";
    di.base = atoi(di_str.substr(0, di_str.find(plus)).c_str());
    di.dice = atoi(di_str.substr(di_str.find(plus)+1, di_str.find(d_d)).c_str());
    di.sides = atoi(di_str.substr(di_str.find(d_d)+1, di_str.length()-1).c_str());
    hp = di;
  }

  void add_color(std::string col){
    if(col == "RED")
      color = RED;
    else if(col == "GREEN")
      color = GREEN;
    else if(col == "BLUE")
      color = BLUE;
    else if(col == "CYAN")
      color = CYAN;
    else if(col == "YELLOW")
      color = YELLOW;
    else if(col == "MAGENTA")
      color = MAGENTA;
    else if(col == "WHITE")
      color = WHITE;
    else if(col == "BLACK")
      color = BLACK;
  }

  void add_abil(std::string abil_str){
    //SMART  TELE  TUNNEL   ERRATIC   PASS  PICKUP  DESTROY   UNIQ  BOSS
    //0      1     2        3         4     5       6         7     8
    abil_string = abil_str;
    if (abil_str.find("SMART") != std::string::npos) {
      abil |= 1UL << 0;
    }
    if (abil_str.find("TELE") != std::string::npos) {
      abil |= 1UL << 1;
    }
    if (abil_str.find("TUNNEL") != std::string::npos) {
      abil |= 1UL << 2;
    }
    if (abil_str.find("ERRATIC") != std::string::npos) {
      abil |= 1UL << 3;
    }
    if (abil_str.find("PASS") != std::string::npos) {
      abil |= 1UL << 4;
    }
    if (abil_str.find("PICKUP") != std::string::npos) {
      abil |= 1UL << 5;
    }
    if (abil_str.find("DESTROY") != std::string::npos) {
      abil |= 1UL << 6;
    }
    if (abil_str.find("UNIQ") != std::string::npos) {
      abil |= 1UL << 7;
    }
    if (abil_str.find("BOSS") != std::string::npos) {
      abil |= 1UL << 8;
    }
  }

  void print_monster(){
    cout<<name << '\n';
    cout<<desc;
    cout<<'.'<< '\n';
    switch(color){
    case RED:
      cout<<"RED"<< '\n';
      break;
    case GREEN:
      cout<<"GREEN"<< '\n';
      break;
    case BLUE:
      cout<<"BLUE"<< '\n';
      break;
    case CYAN:
      cout<<"CYAN"<< '\n';
      break;
    case YELLOW:
      cout<<"YELLOW"<< '\n';
      break;
    case MAGENTA:
      cout<<"MAGENTA"<< '\n';
      break;
    case WHITE:
      cout<<"WHITE"<< '\n';
      break;
    case BLACK:
      cout<<"BLACK"<< '\n';
      break;
    }
    speed.print_dice();
    cout << '\n';
    cout<<abil_string<< '\n';
    hp.print_dice();
    cout<<'\n';
    dam.print_dice();
    cout << '\n';
    cout<< symb << '\n';
    cout<<rrty<< '\n';
  }
};

int main(int argc, char *argv[])
{
  vector<monster> monster_templates;
  ifstream in("monster_desc.txt");

  if(!in) {
    cout << "Cannot open input file.\n";
    return 1;
  }
  std::string str;
  std::getline(in, str);
  if(str != "RLG327 MONSTER DESCRIPTION 1")
    exit(0);
  while (std::getline(in, str)) {
    // output the line
    monster m;
    std::string first;
    std::string sec;
    if(str == "BEGIN MONSTER"){
      while (std::getline(in, str)) {
	first = str.substr(0, str.find(' '));
	if(first == "NAME" && m.name.empty())
	  m.name = str.substr(str.find(' ')+1);
	else if(first == "DESC" && m.desc.empty()){
	  std::string desc = "";
	  while (std::getline(in, str)) {
	    if(str == ".")
	      break;
	    desc += str;
	    desc += "\n";
	  }
	  m.desc = desc;
	}
	else if(first == "COLOR")
	  m.add_color(str.substr(str.find(' ')+1));
	else if(first == "SPEED")
	  m.add_speed(str.substr(str.find(' ')+1));
	else if(first == "ABIL" )
	  m.add_abil(str.substr(str.find(' ')+1));
	else if(first == "HP" )
	  m.add_hp(str.substr(str.find(' ')+1));
	else if(first == "DAM" )
	  m.add_dam(str.substr(str.find(' ')+1));
	else if(first == "SYMB")
	  m.symb = str.substr(str.find(' ')+1).at(0);
	else if(first == "RRTY")
	  m.rrty = atoi(str.substr(str.find(' ')+1).c_str());
	else if(first == "END")
	  break;
	else
	  break;
      }
      if(m.is_complete())
      monster_templates.push_back(m);
    }

    // now we loop back and get the next line in 'str'
  }

  for(monster& mon:monster_templates){
    mon.print_monster();
    cout<<'\n';
  }
  return 0;


  dungeon d;
  time_t seed;
  struct timeval tv;
  int32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;

  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof (d));

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = 0;
  do_seed = 1;
  save_file = load_file = NULL;
  d.max_monsters = MAX_MONSTERS;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
   * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */

  if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'n':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%hu", &d.max_monsters)) {
            usage(argv[0]);
          }
          break;
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
	    if (!strcmp(argv[++i], "seed")) {
	      do_save_seed = 1;
	      do_save_image = 0;
	    } else if (!strcmp(argv[i], "image")) {
	      do_save_image = 1;
	      do_save_seed = 0;
	    } else {
	      save_file = argv[i];
	    }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  srand(seed);

  io_init_terminal();
  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }

  /* Ignoring PC position in saved dungeons.  Not a bug. */
  config_pc(&d);
  gen_monsters(&d);

  io_display(&d);
  if (!do_load && !do_image) {
    io_queue_message("Seed is %u.", seed);
  }
  while (pc_is_alive(&d) && dungeon_has_npcs(&d) && !d.quit) {
    do_moves(&d);
  }
  io_display(&d);

  io_reset_terminal();

  if (do_save) {
    if (do_save_seed) {
      /* 10 bytes for number, please dot, extention and null terminator. */
      save_file = (char *) malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image) {
      if (!pgm_file) {
	fprintf(stderr, "No image file was loaded.  Using default.\n");
	do_save_image = 0;
      } else {
	/* Extension of 3 characters longer than image extension + null. */
	save_file = (char *) malloc(strlen(pgm_file) + 4);
	strcpy(save_file, pgm_file);
	strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image) {
      free(save_file);
    }
  }

  printf("%s", pc_is_alive(&d) ? victory : tombstone);
  printf("You defended your life in the face of %u deadly beasts.\n"
         "You avenged the cruel and untimely murders of %u "
         "peaceful dungeon residents.\n",
         d.PC->kills[kill_direct], d.PC->kills[kill_avenged]);

  if (pc_is_alive(&d)) {
    /* If the PC is dead, it's in the move heap and will get automatically *
     * deleted when the heap destructs.  In that case, we can't call       *
     * delete_pc(), because it will lead to a double delete.               */
    character_delete(d.PC);
  }

  delete_dungeon(&d);

  return 0;
}
