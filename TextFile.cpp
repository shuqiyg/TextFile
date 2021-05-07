/* W06 LAB
   Name: Shuqi Yang
   Std ID: 132162207
   Email: syang136@myseneca.ca
   Date: 02-26-2021
   -------------------------------------------------------
   I have done all the coding by myself and only copied the
   code that my professor provided to complete my workshops
   and assignments.
 */
#include <iostream>
#include <fstream>
#include <string>
#include "TextFile.h"
#include "cstring.h"
using namespace std;

namespace sdds {
   Line::operator const char*() const {
      return m_value;
   }
   Line& Line::operator=(const char* lineValue) {
      delete[] m_value;
      m_value = new char[strLen(lineValue) + 1];
      strCpy(m_value, lineValue);
      return *this;
   }
   Line::Line() {
      m_value = nullptr;
   }
   Line::~Line() {
      delete[] m_value;
      m_value = nullptr;
   }
   void TextFile::setFilename(const char* fname, bool isCopy) {
       if (isCopy) {
           char* temp = new char[strLen(fname) + 3];
           strCpy(temp, "C_");
           strCat(temp, fname);
           delete[] m_filename;
           m_filename = new char[strLen(fname) + 3];
           strCpy(m_filename, temp);
           delete[] temp;
       }
       else {
           delete[] m_filename;
           m_filename = new char[strLen(fname) + 1];
           strCpy(m_filename, fname);
       }     
   }
   void TextFile::setNoOfLines() {
       ifstream if_obj(m_filename);
       char file_char;      
       while (if_obj) {
            if_obj >>noskipws>> file_char;
            if (file_char == '\n') {
                m_noOfLines++;
           }      
           m_noOfLines++;
       }
       if (m_noOfLines == 0) {
           delete [] m_filename;
           m_filename = nullptr;
       }
   }   
   void TextFile::loadText() {
       if (m_filename != nullptr) {
           delete[] m_textLines;
           m_textLines = new Line[m_noOfLines];
           ifstream if_obj(m_filename);
           string str;
           unsigned valid_read = 0;       
           for (unsigned i = 0; i < m_noOfLines && if_obj; i++) {
               if(getline(if_obj, str, '\n')){         
                   delete[] m_textLines[i].m_value;
                   m_textLines[i].m_value = new char[strLen(str.c_str()) + 1];
                   m_textLines[i] = str.c_str();
                   //strCpy(m_textLines[i].m_value, str.c_str());
                   valid_read++;
               }              
           }
           m_noOfLines = valid_read;
       }
   }
   void TextFile::saveAs(const char* fileName)const {
       ofstream of_obj(fileName);
       for(unsigned i = 0; i < m_noOfLines; i++){
          of_obj << m_textLines[i]<< endl;
      }
   }
   void TextFile::setEmpty() {
       delete[] m_textLines;
       m_textLines = nullptr;
       delete[] m_filename;
       m_filename = nullptr;
       m_noOfLines = 0;
   }
   TextFile::TextFile(unsigned pageSize) {
       setEmpty();
       m_pageSize = pageSize;
   }
   TextFile::TextFile(const char* filename, unsigned pageSize) {
       m_pageSize = pageSize;
       setEmpty();
       if (filename != nullptr) {
           setFilename(filename);
           setNoOfLines();
           loadText();
       }
   }
   TextFile::TextFile(const TextFile& tf) {
       m_pageSize = tf.m_pageSize;
       setEmpty();
       if (tf.m_filename != nullptr) {
           setFilename(tf.m_filename, true);
           tf.saveAs(m_filename);
           setNoOfLines();
           loadText();
       }
   }
   TextFile& TextFile::operator=(const TextFile& tf) {
       if (tf.m_filename != nullptr && this->m_filename != nullptr) {
           delete[] m_textLines;
           m_textLines = nullptr;
           tf.saveAs(this->m_filename);
           setNoOfLines();
           loadText();
       }
       return *this;
   }
   TextFile::~TextFile() {
       delete[] m_filename;
       m_filename = nullptr;
       delete[] m_textLines;
       m_textLines = nullptr;
   }
   std::ostream& TextFile::view(std::ostream& ostr)const {
       if(m_filename != nullptr){ 
           ostr << m_filename << endl;
           for (int i = 0; i < strLen(m_filename); i++) {
               ostr << '=';
           }
           ostr << endl;
           for (unsigned i = 0; i < m_noOfLines; i++) {
               if ((i % m_pageSize) == 0 && i != 0) {                
                   ostr << "Hit ENTER to continue...";
                   cin.ignore(2000,'\n');         
               }
               ostr << m_textLines[i] << endl;          
           }
       }
       return ostr; 
   }
   std::istream& TextFile::getFile(std::istream& istr) {
       string fname;
       istr >> fname;
       istr.ignore(2000, '\n');
       setFilename(fname.c_str());
       setNoOfLines();
       loadText();
       return istr;
   }
   TextFile::operator bool()const {
       return m_filename != nullptr;
   }
   unsigned TextFile::lines()const {
       return m_noOfLines;
   }
   const char* TextFile::name()const {
       return m_filename;
   }
   const char* TextFile::operator[](unsigned index)const {
       if (m_filename != nullptr) {
           if (index > m_noOfLines - 1) {
               index = index % m_noOfLines;
           }
           return m_textLines[index];
       }
       else {
           return NULL;
       }
   }
   std::ostream& operator<<(std::ostream& ostr, const TextFile& text) {
       return text.view(ostr);
    }
   std::istream& operator>>(std::istream& istr, TextFile& text) {
       return text.getFile(istr);
    }
}