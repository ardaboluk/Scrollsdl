
//This header contains loader classes

#ifndef LOADER_H__
#define LOADER_H__

extern const int BRICK_WIDTH;
extern const int BRICK_HEIGHT;

extern std::vector<SDL_Texture*> brickimages;
extern std::vector<SDL_Texture*> enemyimages;

extern std::vector<Brick*> bricks;
extern std::vector<Enemy*> enemies;

class Brickloader
{
private:

	std::string filename;
	char delimiter;
	int h_tile_count;	//horizonital tile count
	int v_tile_count;	//vertical tile count

	//splits p_line using delimiter and returns vector of splitted strings
	std::vector<std::string> split(std::string p_line);

public:
	Brickloader(std::string p_filename, char p_delimiter, int p_h_tile_count, int p_v_tile_count);
	bool load_bricks();		//fills the bricks vector
	
};

Brickloader::Brickloader(std::string p_filename, char p_delimiter, int p_h_tile_count, int p_v_tile_count)
{
	filename = p_filename;
	delimiter = p_delimiter;
	h_tile_count = p_h_tile_count;
	v_tile_count = p_v_tile_count;
}

std::vector<std::string> Brickloader::split(std::string p_line)
{
	//This vector will be returned
	std::vector<std::string> vector_string_return;

	std::stringstream line_stream;
	line_stream << p_line;

	std::string string_brick;
	while(!line_stream.eof())
	{
		std::getline(line_stream, string_brick, delimiter);		//string_brick holds one string that represents a brick
		vector_string_return.push_back(string_brick);
	}
	
	return vector_string_return;
}

//Read the map file and initialize bricks accordingly
//gt:greentop;gd:greendown;bm:brownmiddle;gl:greenleft;gr:greenright;
//gclu:greencornerleftup;gcld:greencornerleftdown;gcru:greencornerrightup;
//gcrd:greencornerrightdown
//numbers not used because i could add new types of bricks gradually
//and numbers should be updated accordingly
bool Brickloader::load_bricks()
{
	std::string string_line;	//one line from the map file
	std::vector<std::string> line_strings_vector;	//holds strings, delimited by the delimiter, from one line from the map file
	std::ifstream map_file(filename);

	Brick* newBrick;

	int lineCounter = 0;	//holds the index of the current line (e.g. 0 for the first line)

	if(map_file.is_open())
	{
		while(map_file.good())
		{
			std::getline(map_file, string_line);
			line_strings_vector = split(string_line);

			for(Uint32 counter = 0; counter < line_strings_vector.size(); counter++)
			{
				std::string brick_string = line_strings_vector[counter];

				if(brick_string.compare("0") != 0 && brick_string.compare("") != 0)	//if the string is 0, than there is no brick at that position
				{

					newBrick = new Brick();
					newBrick->set_box(counter * BRICK_WIDTH, lineCounter * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT);
					
					if(brick_string.compare("gt") == 0)
					{
						newBrick->set_image(brickimages[0]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gd") == 0)
					{
						newBrick->set_image(brickimages[1]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("bm") == 0)
					{
						newBrick->set_image(brickimages[2]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gl") == 0)
					{
						newBrick->set_image(brickimages[3]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gr") == 0)
					{
						newBrick->set_image(brickimages[4]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gclu") == 0)
					{
						newBrick->set_image(brickimages[5]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gcld") == 0)
					{
						newBrick->set_image(brickimages[6]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gcru") == 0)
					{
						newBrick->set_image(brickimages[7]);
						bricks.push_back(newBrick);
					}
					else if(brick_string.compare("gcrd") == 0)
					{
						newBrick->set_image(brickimages[8]);
						bricks.push_back(newBrick);
					}
					
				}
			}

			lineCounter++;
		}
	}
	else
	{
		return false;
	}

	return true;
}

class Enemyloader
{
private:

	std::string filename;
	char delimiter;
	int h_tile_count;	//horizonital tile count
	int v_tile_count;	//vertical tile count

	//splits p_line using delimiter and returns vector of splitted strings
	std::vector<std::string> split(std::string p_line);

public:
	Enemyloader(std::string p_filename, char p_delimiter, int p_h_tile_count, int p_v_tile_count);
	bool load_enemies();		//fills the bricks vector

};

Enemyloader::Enemyloader(std::string p_filename, char p_delimiter, int p_h_tile_count, int p_v_tile_count)
{
	filename = p_filename;
	delimiter = p_delimiter;
	h_tile_count = p_h_tile_count;
	v_tile_count = p_v_tile_count;
}

std::vector<std::string> Enemyloader::split(std::string p_line)
{
	//This vector will be returned
	std::vector<std::string> vector_string_return;

	std::stringstream line_stream;
	line_stream << p_line;

	std::string string_brick;
	while(!line_stream.eof())
	{
		std::getline(line_stream, string_brick, delimiter);		//string_brick holds one string that represents a brick
		vector_string_return.push_back(string_brick);
	}
	
	return vector_string_return;
}

//Read the map file and initialize bricks accordingly
//gt:greentop;gd:greendown;bm:brownmiddle;gl:greenleft;gr:greenright;
//gclu:greencornerleftup;gcld:greencornerleftdown;gcru:greencornerrightup;
//gcrd:greencornerrightdown
//numbers not used because i could add new types of bricks gradually
//and numbers should be updated accordingly
bool Enemyloader::load_enemies()
{
	std::string string_line;	//one line from the map file
	std::vector<std::string> line_strings_vector;	//holds strings, delimited by the delimiter, from one line from the map file
	std::ifstream map_file(filename);

	Enemy* newEnemy;

	int lineCounter = 0;	//holds the index of the current line (e.g. 0 for the first line)

	if(map_file.is_open())
	{
		while(map_file.good())
		{
			std::getline(map_file, string_line);
			line_strings_vector = split(string_line);

			for(Uint32 counter = 0; counter < line_strings_vector.size(); counter++)
			{
				std::string enemy_string = line_strings_vector[counter];

				if(enemy_string.compare("0") != 0 && enemy_string.compare("") != 0)	//if the string is 0, than there is no enemy at that position
				{			
					if(enemy_string.compare("e_mn") == 0)
					{
						newEnemy = new Enemy(counter * BRICK_WIDTH, lineCounter * BRICK_HEIGHT, Monster);
						newEnemy->set_images_right(enemyimages[0], enemyimages[1], enemyimages[2], enemyimages[3]);
						newEnemy->set_images_left(enemyimages[4], enemyimages[5], enemyimages[6], enemyimages[7]);
						enemies.push_back(newEnemy);
					}
					else if(enemy_string.compare("e_sk") == 0)
					{
						newEnemy = new Enemy(counter * BRICK_WIDTH, lineCounter * BRICK_HEIGHT, Skeleton);
						newEnemy->set_images_right(enemyimages[8], enemyimages[9], enemyimages[10], enemyimages[11]);
						newEnemy->set_images_left(enemyimages[12], enemyimages[13], enemyimages[14], enemyimages[15]);
						enemies.push_back(newEnemy);
					}
					else if(enemy_string.compare("e_zm") == 0)
					{
						newEnemy = new Enemy(counter * BRICK_WIDTH, lineCounter * BRICK_HEIGHT, Zombie);
						newEnemy->set_images_right(enemyimages[16], enemyimages[17], enemyimages[18], enemyimages[19]);
						newEnemy->set_images_left(enemyimages[20], enemyimages[21], enemyimages[22], enemyimages[23]);
						enemies.push_back(newEnemy);
					}
					else if(enemy_string.compare("e_dr") == 0)
					{
						newEnemy = new Enemy(counter * BRICK_WIDTH, lineCounter * BRICK_HEIGHT, Dragon);
						newEnemy->set_images_right(enemyimages[24], enemyimages[25], enemyimages[26], enemyimages[27]);
						newEnemy->set_images_left(enemyimages[28], enemyimages[29], enemyimages[30], enemyimages[31]);
						enemies.push_back(newEnemy);
					}					
				}
			}

			lineCounter++;
		}
	}
	else
	{
		return false;
	}

	return true;
}

#endif