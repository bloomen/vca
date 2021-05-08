#include "pdf_tokenizer.h"

#include <vca/logging.h>
#include <vca/string.h>
#include <vca/zip_inflater.h>

namespace vca
{

namespace
{

constexpr size_t g_max_byte_count = 8192;

}

PdfTokenizer::PdfTokenizer(std::string entry)
    : m_entry{std::move(entry)}
{
}

std::vector<String>
PdfTokenizer::extract(const fs::path&) const
{
    VCA_INFO << "PDF tokenization not yet supported";
    return {};
}

} // namespace vca
