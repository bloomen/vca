#pragma once

#include <qtc/Handler.h>

namespace vca
{

class SearchHandler : public qtc::Handler
{
public:
    SearchHandler(qtc::Model& model,
                  qtc::ThreadPool& threadPool,
                  qtc::CommandQueue& mainQueue);

    qtc::Node<QString>& m_input = addNode("input", QString{}, false, false);
    qtc::Node<int>& m_result_count =
        addNode("result_count", int{}, false, false);
    qtc::Node<QList<QString>>& m_result_dirs =
        addNode("result_dirs", QList<QString>{}, false, false);
    qtc::Node<QList<QString>>& m_result_files =
        addNode("result_files", QList<QString>{}, false, false);
    qtc::Node<QList<QString>>& m_result_exts =
        addNode("result_exts", QList<QString>{}, false, false);
    qtc::Node<QList<QString>>& m_dir_exclusions =
        addNode("dir_exclusions", QList<QString>{}, true, true);
    qtc::Node<QList<QString>>& m_file_exclusions =
        addNode("file_exclusions", QList<QString>{}, true, true);
    qtc::Node<QList<QString>>& m_ext_exclusions =
        addNode("ext_exclusions", QList<QString>{}, true, true);

    void
    connect() override;
};

} // namespace vca
