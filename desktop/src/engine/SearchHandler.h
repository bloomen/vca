#pragma once

#include <vca/userdb.h>

#include <qtc/Handler.h>

namespace vca
{

class SearchHandler : public qtc::Handler
{
public:
    SearchHandler(qtc::Model& model, qtc::ThreadPool& threadPool, qtc::CommandQueue& mainQueue, const vca::UserDb& user_db);

    qtc::Node<QString>& m_input = addNode("input", QString{}, false, false);
    qtc::Node<QString>& m_result = addNode("result", QString{}, false, false);

    void connect() override;

private:
    const vca::UserDb* m_user_db = nullptr;
};

}
