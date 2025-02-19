#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>


img::EasyImage eRectangle(const ini::Configuration &conf){
        int width = (conf["ImageProperties"]["width"].as_int_or_die());
        int height = (conf["ImageProperties"]["height"].as_int_or_die());
        img::EasyImage image(width,height);
        for(unsigned int i = 0; i < width; i++){
            for(unsigned int j = 0; j < height; j++){
                image(i,j).red = i;
                image(i,j).green = j;
                image(i,j).blue = (i-j)%256;
            }
        }
        return image;
}

img::EasyImage eBlocks(const ini::Configuration &conf){
    int width = (conf["ImageProperties"]["width"].as_int_or_die());
    int height = (conf["ImageProperties"]["height"].as_int_or_die());
    int nrXBlocks = (conf["BlockProperties"]["nrXBlocks"].as_int_or_die());
    int nrYBlocks = (conf["BlockProperties"]["nrYBlocks"].as_int_or_die());
    std::vector<double> colorA = (conf["BlockProperties"]["colorWhite"].as_double_tuple_or_die());
    std::vector<double> colorB = (conf["BlockProperties"]["colorBlack"].as_double_tuple_or_die());
    int modx = width/nrXBlocks;
    int mody = height/nrYBlocks;
    img::EasyImage image(width,height);

    for(unsigned int i = 0; i < width; i++){
        for(unsigned int j = 0; j < height; j++){
            if(((i/modx) + (j/mody))%2 == 0){
                image(i,j).red = std::lround(colorA[0]*255);
                image(i,j).green = std::lround(colorA[1]*255);
                image(i,j).blue = std::lround(colorA[2]*255);
            }else{
                image(i,j).red = std::lround(colorB[0]*255);
                image(i,j).green = std::lround(colorB[1]*255);
                image(i,j).blue = std::lround(colorB[2]*255);
            }

        }
    }
    return image;
}

img::EasyImage eL2D(const ini::Configuration &conf){

    int width = (conf["General"]["size"].as_int_or_die());
    int height = (conf["General"]["size"].as_int_or_die());
    std::vector<double> backgroundColor = (conf["General"]["backgroundcolor"].as_double_tuple_or_die());
    std::vector<double> lineColor = (conf["2DLSystem"]["color"].as_double_tuple_or_die());
    std::string L2DFileName = (conf["2DLSystem"]["inputfile"].as_string_or_die());
    LParser::LSystem2D LPARSER;
    std::ifstream L2DFile(L2DFileName);
    L2DFile >> LPARSER;
    img::EasyImage image(width,height);
    L2DFile.close();

    for(unsigned int i = 0; i < width; i++){
        for(unsigned int j = 0; j < height; j++){


        }
    }
    return image;
}

img::EasyImage generate_image(const ini::Configuration &conf)
{

    if(conf["General"]["type"].as_string_or_die() == "IntroColorRectangle"){

        return eRectangle(conf);
    }
    else if(conf["General"]["type"].as_string_or_die() == "IntroBlocks"){
        return eBlocks(conf);
    }
    else if(conf["General"]["type"].as_string_or_die() == "2DLSystem"){
        return eL2D(conf);
    }else{
        return img::EasyImage(0, 0);
    }



}


int main(int argc, char const* argv[])
{
        int retVal = 0;
        try
        {
                std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
                if (args.empty()) {
                        std::ifstream fileIn("filelist");
                        std::string filelistName;
                        while (std::getline(fileIn, filelistName)) {
                                args.push_back(filelistName);
                        }
                }
                for(std::string fileName : args)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(fileName);
                                if (fin.peek() == std::istream::traits_type::eof()) {
                                    std::cout << "Ini file appears empty. Does '" <<
                                    fileName << "' exist?" << std::endl;
                                    continue;
                                }
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;
                                        std::cout << "\033[32mGenerated image: " << fileName << "[0m" << std::endl;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cerr << "Could not generate image for " << fileName << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }
        return retVal;
}
