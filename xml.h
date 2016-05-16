#pragma once;

#include <iostream>
#include <string>
#include <sstream>

namespace xml 
{    

    class XmlLine
    {
    public:
        //please do not use blank or special character as key     
        template<class T> void add(const std::string& name, T value) 
        { 
            std::stringstream ss; 
            ss << value;
            m_Text += "    <" + name + ">" + ss.str() + "</" + name + ">\n"; 
        }        
        std::string text() const 
        { 
            return pre() + m_Text + post(); 
        }
    private:
        std::string m_Text;                
        std::string pre()  const { return "  <li>\n"; }
        std::string post() const { return "  </li>"; }
    };

    class XmlMain
    {
    public:
        template<typename T> void add(T item)
        { 
            m_Text += item.text() + "\n\n"; 
        }
        std::string text() const
        {
            return pre() + m_Text + post(); 
        }
    private:
        std::string m_Text;
        std::string pre() const
        {
            auto t = std::string();
            t += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            t += "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
            t += "\n";
            t += "<head>\n";
            t += "<title>Beispiel</title>\n";
            t += "</head>\n";
            t += "\n";
            t += "<body>\n";
            t += "<ol>\n";
            t += "\n";
            return t;
        }
        std::string post() const
        {
            auto t = std::string();
            t += "\n";
            t += "</ol>\n";
            t += "</body>\n";
            t += "</html>\n";
            return t;
        }
    };

}
