#include "Dialog.hpp"

#include <cassert>

#include "../util/FromString.hpp"

namespace Game
{

namespace Detail
{

/**
 *
 **/
std::string getDialogString(std::ifstream& ifs, std::string& str)
{
   std::string dialog;
   while(std::getline(ifs, str))
   {
      if(str.compare("{") == 0)
      {
         break;
      }
   }
   while(std::getline(ifs, str))
   {
      if(str.compare("}") == 0)
      {
         break;
      }

      dialog += str;
   }

   return dialog;
}

/**
 * Get Dialog index
 **/
int getDialogIndex(std::ifstream& ifs, std::string& str)
{
   std::getline(ifs, str);
   int dialog_index; 
   if((str[0] == '[') && (str[str.size() - 1] == ']'))
   {
      str = str.erase(str.size() - 1);
      str = str.erase(0, 1);

      dialog_index = Util::fromString<int>(str);
   }
   else
   {
      std::cout << " FALSE: '" << str << "'" << std::endl;
      assert(false);
   }

   return dialog_index;
}

/**
 *
 **/
std::tuple<int, int, int> getDialogResponseIndex(std::ifstream& ifs, std::string& str)
{
   int dialog_index, response_index, next_index;

   dialog_index   = Util::fromString<int>(str.substr(str.find('(') + 1, str.find('.') - 1));
   response_index = Util::fromString<int>(str.substr(str.find('.') + 1, str.find(')') - 3));
   next_index     = Util::fromString<int>(str.substr(str.find(')') + 2, str.size() - 1));

   return std::tie(dialog_index, response_index, next_index);
}

} /* namespace Detail */

struct ResponseStruct
{
   int dialog_index;
   int response_index;
   int next_index;
   std::string response_string;
};

struct DialogStruct
{
   int                   dialog_index;
   std::string           dialog_string;
   std::vector<ResponseStruct> responses;
};

void DialogContainer::load(const std::string& dialog_name) //, const std::string& dialog_path)
{
   // Open dialog file
   std::ifstream dialog_file{"dialogs/" + dialog_name};

   std::vector<DialogStruct> dialog_struct_container;
   
   // Loop over lines
   std::string str;
   while(std::getline(dialog_file, str))
   {
      if(str.compare("[") == 0)
      {
         DialogStruct dialog;

         dialog.dialog_index  = Detail::getDialogIndex (dialog_file, str);
         dialog.dialog_string = Detail::getDialogString(dialog_file, str);

         while(std::getline(dialog_file, str))
         {
            if(str.compare("]") == 0)
            {
               break;
            }

            dialog.responses.emplace_back();
            auto& response = dialog.responses.back();

            std::tie(response.dialog_index, response.response_index, response.next_index) = Detail::getDialogResponseIndex(dialog_file, str);
            response.response_string = Detail::getDialogString(dialog_file, str);
         }

         dialog_struct_container.emplace_back(dialog);
      }
   }

   // Construct dialog from dialog structs
   this->container.resize(dialog_struct_container.size());

   for(const auto& dialog_struct : dialog_struct_container)
   {
      auto& dialog  = this->container[dialog_struct.dialog_index];
      dialog.dialog = dialog_struct.dialog_string;
      
      dialog.choices.resize(dialog_struct.responses.size());
      for(const auto& response_struct : dialog_struct.responses)
      {
         auto& choice = dialog.choices[response_struct.response_index];
         choice.response = response_struct.response_string;
         if(response_struct.next_index >= 0)
         {
            choice.next = &(this->container[response_struct.next_index]);
         }
         else
         {
            choice.next = nullptr;
         }
      }
   }
}

} /* namespace Game */
