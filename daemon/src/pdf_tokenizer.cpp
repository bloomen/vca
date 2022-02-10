#include "pdf_tokenizer.h"

#include <stack>

#include <vca/logging.h>
#include <vca/string.h>
#include <vca/zip_inflater.h>

#include <podofo/podofo.h>

using namespace PoDoFo;

namespace vca
{

namespace
{

constexpr size_t g_max_byte_count = 8192;

std::string
get_utf8(PdfFont* font, const PdfString& str)
{
    if (!font)
    {
        VCA_WARN << "Got text but no font: " << str.GetString();
        return {};
    }

    if (!font->GetEncoding())
    {
        VCA_WARN << "Got text but no encoding: " << str.GetString();
        return {};
    }
    const auto unicode = font->GetEncoding()->ConvertToUnicode(str, font);
    return unicode.GetStringUtf8();
}

std::list<std::string>
extract_text(const Path& path)
{
    size_t byte_count = 0;
    std::list<std::string> words;
    PdfMemDocument doc{path.to_narrow().c_str()};
    const auto n = doc.GetPageCount();
    for (int i = 0; i < n; i++)
    {
        const auto page = doc.GetPage(i);
        const char* token = nullptr;
        PdfVariant var;
        EPdfContentsType type;
        PdfContentsTokenizer tokenizer(page);

        bool is_text = false;
        PdfFont* font = nullptr;

        std::stack<PdfVariant> stack;

        while (tokenizer.ReadNext(type, token, var))
        {
            if (byte_count > g_max_byte_count)
            {
                return words;
            }
            switch (type)
            {
            case ePdfContentsType_Keyword:
            {
                if (strcmp(token, "l") == 0 || strcmp(token, "m") == 0)
                {
                    if (stack.size() == 2)
                    {
                        stack.pop();
                        stack.pop();
                    }
                    else
                    {
                        VCA_WARN << "Token '" << token
                                 << "' expects two arguments, but "
                                 << stack.size() << " given; ignoring";
                        while (!stack.empty())
                        {
                            stack.pop();
                        }
                    }
                }
                else if (strcmp(token, "BT") == 0)
                {
                    is_text = true;
                }
                else if (strcmp(token, "ET") == 0)
                {
                    if (!is_text)
                    {
                        VCA_WARN << "Found ET without BT!";
                    }
                }
                if (is_text)
                {
                    if (strcmp(token, "Tf") == 0)
                    {
                        if (stack.size() < 2)
                        {
                            VCA_WARN << "Expects two arguments for 'Tf', "
                                        "ignoring";
                            font = nullptr;
                            continue;
                        }

                        stack.pop();
                        PdfName fontName = stack.top().GetName();
                        auto f =
                            page->GetFromResources(PdfName("Font"), fontName);
                        if (!f)
                        {
                            VCA_ERROR << "Cannot create font";
                            return words;
                        }

                        font = doc.GetFont(f);
                        if (!font)
                        {
                            VCA_WARN << "Unable to create font for object";
                        }
                    }
                    else if (strcmp(token, "Tj") == 0 ||
                             strcmp(token, "'") == 0)
                    {
                        if (stack.size() < 1)
                        {
                            VCA_WARN << "Expects one argument for '" << token
                                     << "', ignoring";
                            continue;
                        }

                        auto res = get_utf8(font, stack.top().GetString());
                        if (!res.empty() && res != " ")
                        {
                            byte_count += res.size();
                            words.emplace_back(std::move(res));
                        }
                        stack.pop();
                    }
                    else if (strcmp(token, "\"") == 0)
                    {
                        if (stack.size() < 3)
                        {
                            VCA_WARN << "Expects three arguments for '" << token
                                     << "', ignoring";

                            while (!stack.empty())
                            {
                                stack.pop();
                            }
                            continue;
                        }

                        auto res = get_utf8(font, stack.top().GetString());
                        if (!res.empty() && res != " ")
                        {
                            byte_count += res.size();
                            words.emplace_back(std::move(res));
                        }

                        stack.pop();
                        stack.pop(); // remove char spacing from stack
                        stack.pop(); // remove word spacing from stack
                    }
                    else if (strcmp(token, "TJ") == 0)
                    {
                        if (stack.size() < 3)
                        {
                            VCA_WARN << "Expects one argument for '" << token
                                     << "', ignoring";
                            continue;
                        }

                        const auto array = stack.top().GetArray();
                        stack.pop();

                        std::string res;
                        for (const auto& e : array)
                        {
                            if (e.IsString() || e.IsHexString())
                            {
                                auto s = get_utf8(font, e.GetString());
                                if (s != " ")
                                {
                                    res += s;
                                }
                                else if (!res.empty())
                                {
                                    byte_count += res.size();
                                    words.emplace_back(std::move(res));
                                    res.clear();
                                }
                            }
                        }
                        if (!res.empty())
                        {
                            byte_count += res.size();
                            words.emplace_back(std::move(res));
                        }
                    }
                }
            }
            break;
            case ePdfContentsType_Variant:
            {
                stack.push(var);
            }
            break;
            case ePdfContentsType_ImageData:
                break;
            }
        }
    }
    return words;
}

} // namespace

std::vector<String>
PdfTokenizer::extract(const Path& file) const
{
    auto words = extract_text(file);
    for (auto w = words.begin(); w != words.end(); ++w)
    {
        if (w != words.end() && w->size() > 1 && w->back() == '-')
        {
            auto start = w;
            ++w;
            start->erase(start->size() - 1);
            (*start) += *w;
            words.erase(w);
        }
    }
    String one_line;
    for (const auto& w : words)
    {
        one_line += U" " + narrow_to_wide(w);
    }
    return tokenize(std::move(one_line));
}

} // namespace vca
