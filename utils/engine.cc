#include "easy_image.h"

#include "ini_configuration.h"
#include "l_parser.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

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
    img::Color bgColor(std::lround(backgroundColor[0]*255),std::lround(backgroundColor[1]*255),std::lround(backgroundColor[2]*255));
    img::Color drawColor(std::lround(lineColor[0]*255),std::lround(lineColor[1]*255),std::lround(lineColor[2]*255));
    //
    //laad de L2D file in en slaag die op in LPARSER
    //
    LParser::LSystem2D LPARSER;
    std::ifstream L2DFile(L2DFileName);
    L2DFile >> LPARSER;
    img::EasyImage image(width,height);
    L2DFile.close();

    //
    //haal het alphabet
    //
    std::set<char> alphabet = LPARSER.get_alphabet();

    //
    //main loop
    //
    std::string mainstring = LPARSER.get_initiator();
    std::string tempstring;
    for(int i = 0; i < LPARSER.get_nr_iterations(); i++){
        tempstring = "";
        for(char c: mainstring)
        {
            if(alphabet.find(c) != alphabet.end()){
                tempstring += LPARSER.get_replacement(c);
            }else{
                tempstring += c;
            }
        }
        mainstring = tempstring;
    }
    double currentX = 0;
    double currentY = 0;
    double nextX = 0;
    double nextY = 0;
    double currentAngle = LPARSER.get_starting_angle() * (M_PI / 180);
    double angle = LPARSER.get_angle() * (M_PI / 180);
    std::vector<img::Line2D> lines;
    std::stack<double> postionX;
    std::stack<double> postionY;
    std::stack<double> angleStack;
    for(char c: mainstring){
        if(alphabet.find(c) != alphabet.end()){
            if(LPARSER.draw(c) != false){
                nextX = currentX + LPARSER.draw(c) * cos(currentAngle);
                nextY = currentY + LPARSER.draw(c) * sin(currentAngle);
                img::Line2D newLine;
                newLine.p1.x = currentX;
                newLine.p1.y = currentY;
                newLine.p2.x = nextX;
                newLine.p2.y = nextY;
                newLine.color = drawColor;
                lines.push_back(newLine);
                currentX = nextX;
                currentY = nextY;
            }else{
                currentX = currentX + LPARSER.draw(c) * cos(currentAngle);
                currentY = currentY + LPARSER.draw(c) * sin(currentAngle);
            }

        }
        if(c == '+'){
            currentAngle += angle;
        }
        else if(c == '-'){
            currentAngle -= angle;
        } else if(c == '('){
            postionX.push(currentX);
            postionY.push(currentY);
            angleStack.push(currentAngle);
        }else if(c == ')'){
            currentX = postionX.top();
            currentY = postionY.top();
            currentAngle = angleStack.top();
            postionX.pop();
            postionY.pop();
            angleStack.pop();
        }


    }
    return img::EasyImage::draw2DLines(lines, width, height, bgColor);


    //
    //return the image
    //

}



img::EasyImage eL2DST(const ini::Configuration &conf){
    int width = (conf["General"]["size"].as_int_or_die());
    int height = (conf["General"]["size"].as_int_or_die());
    std::vector<double> backgroundColor = (conf["General"]["backgroundcolor"].as_double_tuple_or_die());
    std::vector<double> lineColor = (conf["2DLSystem"]["color"].as_double_tuple_or_die());
    std::string L2DFileName = (conf["2DLSystem"]["inputfile"].as_string_or_die());
    img::Color bgColor(std::lround(backgroundColor[0]*255),std::lround(backgroundColor[1]*255),std::lround(backgroundColor[2]*255));
    img::Color drawColor(std::lround(lineColor[0]*255),std::lround(lineColor[1]*255),std::lround(lineColor[2]*255));
    //
    //laad de L2D file in en slaag die op in LPARSER
    //
    LParser::LSystem2D LPARSER;
    std::ifstream L2DFile(L2DFileName);
    L2DFile >> LPARSER;
    img::EasyImage image(width,height);
    L2DFile.close();

    LPARSER.get_replacement('A');
    return image;


    //
    //return the image
    //

}

img::EasyImage WireFrame(const ini::Configuration &conf){
    int width = (conf["General"]["size"].as_int_or_die());
    int height = (conf["General"]["size"].as_int_or_die());
    std::vector<double> backgroundColor = (conf["General"]["backgroundcolor"].as_double_tuple_or_die());
    int numberOfFigures = (conf["General"]["nrFigures"].as_int_or_die());
    std::vector<double> eye = (conf["General"]["eye"].as_double_tuple_or_die());
    std::vector<img::Figure> figures;
    for(int i = 0; i < numberOfFigures; i++){
        auto figure = (conf["Figure" + std::to_string(i)]);
        /*
                rotateX = 0
                rotateY = 0
                rotateZ = 0
                scale = 1.0
                center = (0, 0, 0)
                color = (0.0, 1.0, 0.0)
                nrPoints = 14
                nrLines = 28
         */
        int rorateXVal = figure["rotateX"].as_int_or_die();
        int rorateYVal = figure["rotateY"].as_int_or_die();
        int rorateZVal = figure["rotateZ"].as_int_or_die();
        auto scaleVal = figure["scale"].as_double_or_die();
        std::vector<double> color = (figure["color"].as_double_tuple_or_die());
        std::vector<double> center = (figure["center"].as_double_tuple_or_die());
        int nrPoints = figure["nrPoints"].as_int_or_die();
        int nrLines = figure["nrLines"].as_int_or_die();
        img::Figure newFigure;
        newFigure.points = std::vector<Vector3D>();
        newFigure.faces = std::vector<img::Face>();
        newFigure.color = img::Color(std::lround(color[0]*255),std::lround(color[1]*255),std::lround(color[2]*255));

        for(int j = 0; j < nrPoints; j++){
            std::vector<double> point = (figure["point" + std::to_string(j)]);
            Vector3D newPoint = Vector3D::point(point[0],point[1],point[2]);
            newFigure.points.push_back(newPoint);
        }

        for(int j = 0; j < nrLines; j++){
            std::vector<int> line = (figure["line" + std::to_string(j)]);
            img::Face newFace;
            newFace.point_indexes = line;
            newFigure.faces.push_back(newFace);
        }

        Matrix transformMatrix = img::EasyImage::scaleFigure(scaleVal) * img::EasyImage::rotateX(rorateXVal) * img::EasyImage::rotateY(rorateYVal) * img::EasyImage::rotateZ(rorateZVal);
        img::EasyImage::applyTransformation(newFigure, transformMatrix);


        Matrix eyeMatrix = img::EasyImage::eyePointTrans(Vector3D::point(eye[0], eye[1], eye[2]));
        img::EasyImage::applyTransformation(newFigure, eyeMatrix);

        Matrix transMatrix =  img::EasyImage::translate(Vector3D::point(center[0],center[1],center[2]));
        img::EasyImage::applyTransformation(newFigure, transMatrix);
        figures.push_back(newFigure);
    }



    std::vector<img::Line2D> projectedLines = img::EasyImage::doProjection(figures);
    return img::EasyImage::draw2DLines(projectedLines, width, height, img::Color(backgroundColor[0], backgroundColor[1], backgroundColor[2]));


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
    }
    else if(conf["General"]["type"].as_string_or_die() == "2DLSystemST"){
        return eL2DST(conf);

    }else if(conf["General"]["type"].as_string_or_die() == "Wireframe"){
        return WireFrame(conf);

    }else{

        img::EasyImage image;
        return image;
    }


}


int main(int argc, char const* argv[])
{
        int retVal = 0;
        try
        {
                std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
                if (args.empty()) {
                        auto path = std::filesystem::current_path(); //getting path
                        std::cout << path << std::endl;

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
                                        fileName = "../build/" + fileName;
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
