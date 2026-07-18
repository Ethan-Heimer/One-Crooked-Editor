#pragma once

#include <sstream>

#include "gapbuffer.h"

#include "doublyindexedlinkedlist.h"
#include "ieditable.h"

namespace Buffers{

    class BufferData final : public DoublyIndexedLinkedList<GapBuffer>, public Editor::ILineCollection{
        public:
            struct LineData final : Editor::ILineData{
                std::shared_ptr<Node> line;
                LineData(std::shared_ptr<Node> line) : line(line){};

                std::shared_ptr<Editor::ILineData> NextLine() const override{
                    if(!line)
                        return nullptr;

                    return std::make_shared<LineData>(line->next);
                };

                std::shared_ptr<Editor::ILineData> PreviousLine() const override{
                    if(!line)
                        return nullptr;

                    return std::make_shared<LineData>(line->previous.lock());
                };

                std::string ToString() const override{
                    if(!line)
                        return "";

                    return line->data->ToString();
                }

                void* GetLineAddress() const override{
                    if(!line)
                        return nullptr;

                    return line.get();
                }
            };


            std::shared_ptr<Node> currentLine{head}; 
            Editor::LineIterator Begin() override{
                return Editor::LineIterator{make_shared<LineData>(head)};
            };

            Editor::LineIterator BeginAtCurrentLine() override{
                return Editor::LineIterator{make_shared<LineData>(currentLine)};
            };

            Editor::LineIterator BeginStepsFromCurrentLine(int steps) override{
                std::shared_ptr<Node> currentNode = currentLine;
                int i = 0;
                if(steps < 0){
                    while(currentNode->previous.lock() && i < steps) {
                        currentNode = currentNode->previous.lock(); 
                        i++;
                    }
                } else {
                    while(currentNode->next && i < steps) {
                        currentNode = currentNode->next; 
                        i++;
                    }

                }

                return Editor::LineIterator{make_shared<LineData>(currentNode)};
            };

            Editor::LineIterator End() override{
                if(!tail)
                    return Editor::LineIterator{make_shared<LineData>(nullptr)};

                return Editor::LineIterator{make_shared<LineData>(tail->next)};
            }

            Editor::LineIterator EndStepsFromCurrentLine(unsigned int steps) override{
                std::shared_ptr<Node> currentNode = currentLine;
                int i = 0;
                while(currentNode->next && i < steps) {
                    currentNode = currentNode->next; 
                    i++;
                }

                return Editor::LineIterator{make_shared<LineData>(currentNode->next)};
            };
    };

    using BufferNode = shared_ptr<BufferData::Node>;
    class Buffer : public Editor::IEditable{
        public:
            BufferData buffer; 
            Buffer();

            void ReadLineFromFile(const std::string& line) override;
            std::stringstream WriteLinesToFile() override;

            void GotoNextLine() noexcept override;
            
            void GotoPreviousLine() noexcept override;
            
            void MoveCursorLeft() noexcept override;
            
            void MoveCursorRight() noexcept override;
            
            bool IsCursorAtBeginningOfLine() noexcept override;
            
            void InsertCharacter(char character) noexcept override;
            void InsertCharacterAt(unsigned index, char character) noexcept override;

            void InsertString(string_view character) noexcept override;
            
            void InsertLine() noexcept override;

            void DeleteCharacterAt(unsigned int index) noexcept override;
            void DeleteCharacter() noexcept override; 
            
            void DeleteLine() noexcept override;

            void AppendTextToNextLine() noexcept override;

            void MoveToHead() noexcept override;

            virtual Editor::LineIterator Begin() override;
            virtual Editor::LineIterator BeginAtCurrentLine() override;
            virtual Editor::LineIterator BeginStepsFromCurrentLine(int steps) override;

            virtual Editor::LineIterator End() override;
            virtual Editor::LineIterator EndStepsFromCurrentLine(unsigned int steps) override;

            unsigned int GetCursorX() noexcept override;
            unsigned int GetCurrentLineNumber() noexcept override;
    };
}
