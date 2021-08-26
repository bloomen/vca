#pragma once

#include <vca/userdb.h>

#include <qtc/Handler.h>

namespace vca
{

class SearchHandler : public qtc::Handler
{
public:
    SearchHandler(qtc::Model& model,
                  qtc::ThreadPool& threadPool,
                  qtc::CommandQueue& mainQueue,
                  const vca::UserDb& user_db);

    qtc::Node<QString>& m_input = addNode("input", QString{}, false, false);
    qtc::Node<int>& m_result_count =
        addNode("result_count", int{}, false, false);
    qtc::Node<QList<QString>>& m_result_dirs =
        addNode("result_dirs", QList<QString>{}, false, false);
    qtc::Node<QList<QString>>& m_result_files =
        addNode("result_files", QList<QString>{}, false, false);
    qtc::Node<QList<QString>>& m_result_exts =
        addNode("result_exts", QList<QString>{}, false, false);

    void
    connect() override;

private:
    const vca::UserDb* m_user_db = nullptr;
};

} // namespace vca
