
#include "atmo_driver.hh"
#include "src/symboltable/symboltable.hh"
#include <fstream>
// https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison

void Process_Args( std::vector<std::string>& params, AtmoDriver& driver)
{
    for(auto param : params)
    {
        if (param == "df")
        {
            driver.set_lexer_debug_level(1);
        }
        else if(param == "db")
        {
            driver.set_parser_debug_level(1);
        }
        
        
    }
    

}

int main(int argc, char** argv)
{
    AtmoDriver driver;
    std::ifstream input("test.txt");
    // + 1 because first arg is app name
    std::vector<std::string> params(argv + 1, argv+argc);
    
    Process_Args(params,driver);
    driver.StartCompilation(input);
    
    return 0;
}
