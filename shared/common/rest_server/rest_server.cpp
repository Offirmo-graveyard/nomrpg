#include "rest_server.hpp"

#include <sstream>


void RestResponse::addBodyContent(const std::string& text)
{
    bodyContent_ += text;
}
void RestResponse::addBodyContent(const std::ostringstream& textStream)
{
    bodyContent_ += textStream.str();
}
