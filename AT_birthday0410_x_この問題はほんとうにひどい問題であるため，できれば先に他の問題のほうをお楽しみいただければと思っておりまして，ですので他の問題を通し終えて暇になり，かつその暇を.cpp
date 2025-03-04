/**
 * @link https://www.luogu.com.cn/problem/AT_birthday0410_x
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
#include "./libs/fixed_int.hpp"

using f64 = double;

using namespace lib;

std::array<i32, 9> dx8 {  1,  1,  0, -1, -1, -1,  0,  1  };
std::array<i32, 9> dy8 {  0,  1,  1,  1,  0, -1, -1, -1  };

f64 constexpr pi =  3.141592653589793;

char const _font[16][65][39] = {
{"f", "f", "f", "f", "f", "f", "f", "OHO", "MLM", "KPK", "JRJ", "ITI", "HVH", "GXG", "GJDJG", "FIHIF", "FHJHF", "EIJIE", "EHLHE", "EHLHE", "EHLHE", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "DHNHD", "EHLHE", "EHLHE", "EHLHE", "EIJIE", "FHJHF", "FIHIF", "GJDJG", "GXG", "HVH", "ITI", "JRJ", "KPK", "MLM", "OHO", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "SDO", "QGN", "NJN", "KMN", "HPN", "GQN", "GQN", "GQN", "GQN", "HFBHN", "HCEHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "PHN", "GZE", "F[E", "F[E", "F[E", "F[E", "GZE", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "NIO", "KNM", "HSK", "GUJ", "FWI", "FWI", "FXH", "FIFIH", "FGIIG", "FGJHG", "FGJHG", "FGJHG", "FGJHG", "GFJHG", "VIG", "VHH", "UIH", "TJH", "SJI", "RKI", "QKJ", "PKK", "OKL", "NKM", "MKN", "LKO", "KKEEF", "JKEGE", "IKFGE", "HKGGE", "GKHGE", "FKIGE", "E\\E", "E\\E", "E\\E", "E\\E", "E\\E", "F[E", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "NJN", "JPL", "HTJ", "GVI", "GWH", "GXG", "GXG", "GHGJF", "GGIIF", "GGJHF", "GGJHF", "HFJHF", "XHF", "XHF", "WHG", "VIG", "OOH", "NOI", "NNJ", "NOI", "NPH", "OPG", "VJF", "XHF", "XIE", "YHE", "YHE", "YHE", "YHE", "XIE", "GCMJE", "FHHJF", "FZF", "FYG", "EZG", "FXH", "FVJ", "IQL", "LKO", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "UEL", "TGK", "SHK", "RIK", "QJK", "PKK", "PKK", "OLK", "NMK", "MNK", "LOK", "KPK", "KHAGK", "JHBGK", "IHCGK", "HHDGK", "GIDGK", "GHEGK", "FHFGK", "EHGGK", "D^D", "D^D", "D^D", "D^D", "D^D", "D^D", "SHK", "SHK", "SHK", "SHK", "SHK", "SHK", "MTE", "LUE", "LUE", "LUE", "LUE", "MTE", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "f", "HVH", "HWG", "HWG", "HWG", "HWG", "HVH", "HGW", "HGW", "HGW", "HGW", "HGW", "HGW", "GHBHM", "GTK", "GVI", "GWH", "GXG", "GXG", "GYF", "GHGJF", "JBKIF", "XIE", "YHE", "YHE", "YHE", "YHE", "YHE", "YHE", "HAOIE", "GDLIF", "FHGKF", "FZF", "FYG", "EYH", "FWI", "GUJ", "IQL", "MJO", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "YFG", "TKG", "QOF", "OQF", "MSF", "LTF", "KTG", "JPL", "ILQ", "HKS", "HIU", "GIV", "GHW", "FHX", "FHX", "FGDHM", "FGBMJ", "EYH", "EZG", "E[F", "E[F", "ELFJE", "EJIIE", "EIKID", "EHMHD", "EHMHD", "EHMHD", "FGMHD", "FGMHD", "FHKID", "FIIIE", "GJEKE", "HYE", "HXF", "IVG", "JTH", "KRI", "MNK", "PHN", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "f", "F[E", "F\\D", "F\\D", "F\\D", "F\\D", "F[E", "FGKIE", "FGKIE", "FGJIF", "FGJIF", "FGJHG", "FGIIG", "FGIHH", "FGHIH", "GEIIH", "UHI", "TII", "THJ", "SIJ", "SHK", "RIK", "RIK", "RHL", "QIL", "QHM", "PIM", "PHN", "PHN", "OHO", "OHO", "NIO", "NHP", "NHP", "MHQ", "MHQ", "MGR", "MGR", "OER", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "OIN", "MML", "KQJ", "JSI", "IUH", "HWG", "HWG", "HIEIG", "GIGIF", "GHIHF", "GHIHF", "GHIHF", "GHIHF", "GHIHF", "HHGHG", "HIEIG", "IUH", "JSI", "KQJ", "KQJ", "IUH", "HWG", "GIFJF", "FHJHF", "FHJIE", "EHLHE", "EHLHE", "EHLHE", "EHLHE", "EIJIE", "EIJIE", "FJFJF", "FZF", "GXG", "GXG", "HVH", "ITI", "KPK", "NJN", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "NIO", "LMM", "JQK", "ISJ", "HUI", "GWH", "FXH", "FJFIG", "FIHIF", "EIJHF", "EHLGF", "EHLGF", "EHLHE", "EHLHE", "EHLHE", "EIJIE", "FHIJE", "FJFKE", "GZE", "GZE", "HYE", "IXE", "KLBHE", "MHDGF", "YGF", "XHF", "XHF", "WHG", "VIG", "TJH", "RLH", "NOI", "ISJ", "HSK", "HRL", "HPN", "HNP", "HLR", "IFW", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "WDK", "VFJ", "THJ", "SJI", "RKI", "QLI", "PLJ", "OKL", "NKM", "MKN", "MJO", "LJP", "LIQ", "KIR", "KHS", "JIS", "JHT", "IIT", "IHU", "IHU", "IHU", "HIU", "HHV", "HHV", "HHV", "HHV", "HHV", "HHV", "HHV", "HHV", "HHV", "HHV", "IHU", "IHU", "IHU", "IIT", "JHT", "JIS", "JIS", "KIR", "KJQ", "LIQ", "MIP", "MJO", "NKM", "OKL", "PKK", "QLI", "RKI", "SJI", "TII", "UGJ", "WDK", "YAL", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "KDW", "JFV", "IIT", "IJS", "IKR", "ILQ", "JLP", "LKO", "MKN", "NJN", "OJM", "PJL", "QIL", "RIK", "SHK", "SIJ", "THJ", "TII", "UHI", "UHI", "UHI", "UIH", "VHH", "VHH", "VHH", "VHH", "VHH", "VHH", "VHH", "VHH", "VHH", "VHH", "UHI", "UHI", "UHI", "TII", "THJ", "SIJ", "SIJ", "RIK", "QJK", "PJL", "PIM", "NKM", "MKN", "LKO", "KKP", "ILQ", "IKR", "IJS", "IIT", "JGU", "KDW", "LAY", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "QEP", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "F[E", "F[E", "F[E", "F[E", "F[E", "F[E", "F[E", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "PGO", "QEP", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "FZF", "E\\E", "E\\E", "E\\E", "E\\E", "E\\E", "F[E", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "f", "f", "f", "f", "QDQ", "PFP", "PGO", "OHO", "OHO", "PGO", "PFP", "GEDFEDG", "GFCFCGF", "FIBEAIF", "FOAJF", "FZF", "FZF", "GXG", "LNL", "OHO", "NKM", "MML", "KHAGK", "JHBHJ", "JHBII", "IHDHI", "IHDHI", "JGEGI", "KEFFJ", "LCIBL", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"},
{"f", "f", "f", "[DG", "[FE", "ZGE", "ZGE", "YHE", "YGF", "XHF", "XGG", "WHG", "WGH", "VHH", "VGI", "UHI", "UHI", "UGJ", "THJ", "TGK", "SHK", "SGL", "RHL", "RGM", "QHM", "QGN", "PHN", "PGO", "OHO", "OGP", "OGP", "NHP", "NGQ", "MHQ", "MGR", "LHR", "LGS", "KHS", "KGT", "JHT", "JGU", "IHU", "IGV", "HHV", "HHV", "HGW", "GHW", "GGX", "FHX", "FGY", "EHY", "EGZ", "FFZ", "GEZ", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f", "f"}
};
char font[16][65][39];
bool specialMode = false;
std::mt19937 rng{745184};
using namespace std::string_literals;

auto initFont() -> void {
    for (i32 i = 0; i < 16; i++) {
        for (i32 j = 0; j < 65; j++) {
            char const *it = std::begin(_font[i][j]);
            i32 k = 0;
            i32 ch_idx = 0;
            for (; *it != '\0'; it++) {
                i32 cnt = *it - 'A' + 1;
                for (i32 _ = cnt; _ --> 0; )  font[i][j][k++] = ".#"[ch_idx];
                ch_idx ^= 1;
            }
            font[i][j][k] = '\0';
        }
    }
}

namespace Solution_5849927751344851 {
    char constexpr black = '#';
    char constexpr white = '.';
    struct ImageTemplate;
    /**
     * 描述一个二维黑白图像。
     */
    class Image {
    public:
        i32 width = 0, height = 0;
        std::vector<std::vector<char>> pixels;  // true / false

        Image(i32 width = 0, i32 height = 0): width(width), height(height), pixels(height, std::vector<char>(width, false)) {}
        Image(std::vector<std::string> const &s) {
            height = s.size();
            width = ranges::max(s | views::transform(lam(x, x.size())));

            pixels.resize(height);
            for (i32 i = 0; i < height; i++) {
                pixels[i].resize(width);
                for (i32 j = 0; j < static_cast<i32>(s[i].size()); j++) {
                    pixels[i][j] = s[i][j] == black;
                }
            }
        }
        Image(std::vector<std::vector<char>> const &pixels): pixels(pixels) {
            height = pixels.size();
            width = height == 0? 0: pixels[0].size();
        }

        auto reduceNoise() const -> Image;
        auto split() const -> std::vector<Image>;
        auto cutRows() const -> Image;
        auto cutColumns() const -> Image;
        auto stretch(f64, f64) const -> Image;
        auto rotate(f64) const -> Image;
        auto match(ImageTemplate &) const -> f64;

        auto operator[] (uz index) -> std::vector<char> & { return pixels[index]; }
        auto operator[] (uz index) const -> std::vector<char> const & { return pixels[index]; }
        auto friend operator<< (std::ostream &os, Image const &image) -> std::ostream & {
            for (auto const &line: image.pixels) {
                for (auto const &pixel: line) {
                    os << (pixel? black: white);
                }
                os << endl;
            }
            return os;
        }
        auto string() const -> std::string {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }
    };

    struct ImageTemplate {
        Image img;

        using floatingPointCompare = decltype([](f64 x, f64 y) -> bool { return x - y < -1e-5; });
        std::map<f64, Image, floatingPointCompare> rotated;

        auto rotate(f64 angle) -> Image const & {
            if (rotated.contains(angle)) {
                return rotated.at(angle);
            } else {
                return rotated[angle] = img.rotate(angle);
            }
        }
    };

    /**
     * 对图像降噪。
     */
    auto Image::reduceNoise() const -> Image {
        std::vector new_img(height, std::vector<char>(width, false));
        for (i32 i = 0; i < height; i++) {
            for (i32 j = 0; j < width; j++) {
                // 采取一个较为简单的策略：周围的黑点不少于 4 个就认为是黑色。

                std::array<i32, 2> cnts;  cnts.fill(0);
                for (i32 k = 0; k < 8; k++) {
                    auto x = i + dx8[k], y = j + dy8[k];
                    if (0 <= x and x < height and 0 <= y and y < width) {
                        cnts[pixels[x][y]]++;
                    }
                }
                cnts[pixels[i][j]]++;
                new_img[i][j] = cnts[1] >= (4 + specialMode);
            }
        }

        // 去除过小的黑色或白色联通块
        auto walk = [&](i32 x, i32 y, auto &vis) -> std::vector<std::pair<i32, i32>> {
            bool color = new_img[x][y];

            std::deque<std::pair<i32, i32>> q;
            std::vector<std::pair<i32, i32>> res;
            q.push_back({x, y});
            while (not q.empty()) {
                auto [x, y] = q.front();  q.pop_front();
                if (vis[x][y])  continue;
                vis[x][y] = true;
                res.push_back({x, y});

                for (i32 k = 0; k < 8; k++) {
                    auto nx = x + dx8[k], ny = y + dy8[k];
                    if (0 <= nx and nx < height and 0 <= ny and ny < width and new_img[nx][ny] == color) {
                        q.push_back({nx, ny});
                    }
                }
            }
            return res;
        };

        auto res_pixels = new_img;
        i32 constexpr maxIgnoreSize = 15;  // 忽略小于等于多少个点的联通块
        std::vector vis(height, std::vector<char>(width));
        for (i32 i = 0; i < height; i++) {
            for (i32 j = 0; j < width; j++) {
                if (not vis[i][j]) {
                    auto block = walk(i, j, vis);
                    if (block.size() < maxIgnoreSize) {
                        for (auto [x, y]: block) {
                            res_pixels[x][y] = not res_pixels[x][y];
                        }
                    }
                }
            }
        }
        
        return Image{res_pixels};
    }

    /**
     * 分割图像。
     * 要求字符之间有一定宽度的几列接近纯白。
     */
    auto Image::split() const -> std::vector<Image> {
        std::vector<Image> res;

        std::vector columns(width, std::vector<char>(height));
        for (i32 i = 0; i < width; i++) {
            for (i32 j = 0; j < height; j++) {
                columns[i][j] = pixels[j][i];
            }
        }

        std::vector<char> is_white(width);  // 每一列是否为纯白
        for (i32 i = 0; i < width; i++) {
            auto cnt = ranges::count(columns[i], true);
            if (cnt * 15 < height)  is_white[i] = true;
        }

        i32 constexpr split_width = 4;  // 至少需要多少列空白
        i32 constexpr block_min_width = 8;  // 最少需要多少列才能成块
        i32 i = 0;  // 从这一列开始不是空白列
        while (i < width) {
            while (i < width and is_white[i])  i++;
            if (i >= width)  break;

            auto next = std::distance(is_white.begin(), std::search_n(is_white.begin() + i, is_white.end(), split_width, true));
            
            if (next - i >= block_min_width) {
                std::vector rows(height, std::vector<char>(next - i));
                for (i32 j = 0; j < height; j++) {
                    for (i32 k = 0; k < next - i; k++) {
                        rows[j][k] = columns[i + k][j];
                    }
                }
                res.push_back(Image{rows});
            }
            
            if (next == width)  break;
            i = next + split_width;
        }

        return res;
    }

    /**
     * 去除上下的空行
     */
    auto Image::cutRows() const -> Image {
        auto is_white = [&](std::vector<char> const &line) -> bool {
            auto cnt = ranges::count(line, true);
            return cnt * 15 < width;
        };

        i32 i = 0, j = height - 1;
        while (i < height and is_white(pixels[i]))  i++;
        while (j >= i and is_white(pixels[j]))  j--;

        std::vector tmp(pixels.begin() + i, pixels.begin() + (j + 1));
        return Image{tmp};
    }
    /**
     * 去除左右的空列
     */
    auto Image::cutColumns() const -> Image {
        std::vector tmp(width, std::vector<char>(height));
        for (i32 i = 0; i < height; i++) {
            for (i32 j = 0; j < width; j++) {
                tmp[j][i] = pixels[i][j];
            }
        }
        auto cutted = Image{tmp}.cutRows();
        std::vector res(cutted.width, std::vector<char>(cutted.height));
        for (i32 i = 0; i < cutted.height; i++) {
            for (i32 j = 0; j < cutted.width; j++) {
                res[j][i] = cutted.pixels[i][j];
            }
        }
        return Image{res};
    }

    /**
     * 拉伸一个图像
     */
    auto Image::stretch(f64 rows_scale, f64 columns_scale) const -> Image {
        auto new_height = static_cast<i32>(std::round(height * rows_scale));
        auto new_width = static_cast<i32>(std::round(width * columns_scale));
        std::vector new_pixels(new_height, std::vector<char>(new_width));

        f64 inv_rows_scale = 1.0 / rows_scale;
        f64 inv_columns_scale = 1.0 / columns_scale;  // 预处理倒数，减少除法次数
        for (i32 i = 0; i < new_height; i++) {
            for (i32 j = 0; j < new_width; j++) {
                f64 x0 = i * inv_rows_scale, y0 = j * inv_columns_scale;
                f64 x1 = x0 + inv_rows_scale, y1 = y0 + inv_columns_scale;

                f64 area = 0;  // 计算黑色的面积
                for (i32 x = x0; x <= x1; x++) {
                    for (i32 y = y0; y <= y1; y++) {
                        if (0 <= x and x < height and 0 <= y and y < width and pixels[x][y]) {
                            auto left = std::max<f64>(y, y0);
                            auto right = std::min<f64>(y + 1, y1);
                            auto top = std::max<f64>(x, x0);
                            auto bottom = std::min<f64>(x + 1, x1);
                            area += (right - left) * (bottom - top);
                        }

                    }
                }
                new_pixels[i][j] = area * 2 > (inv_rows_scale * inv_columns_scale);
            }
        }
        return Image{new_pixels};
    }

    /**
     * 将图像逆时针旋转若干弧度
     */
    auto Image::rotate(f64 rad) const -> Image {
        rad = -rad;
    
        // 调整扩展图像的尺寸
        i32 new_width = std::ceil(std::sqrt(width * width + height * height));
        i32 new_height = new_width;

        Image middle;
        middle.pixels = std::vector(new_height, std::vector<char>(new_width));
        middle.width = new_width, middle.height = new_height;

        // 将原始图像放置在扩展图像的中心
        i32 offset_r = (new_height - height) / 2;
        i32 offset_c = (new_width - width) / 2;

        for (i32 i = 0; i < height; i++) {
            for (i32 j = 0; j < width; j++) {
                middle.pixels[i + offset_r][j + offset_c] = pixels[i][j];
            }
        }

        // 计算旋转中心
        f64 mid_r = new_height / 2.0;
        f64 mid_c = new_width / 2.0;
        f64 cos_r = std::cos(rad), sin_r = std::sin(rad);
    
        std::vector new_pixels(middle.height, std::vector<char>(middle.width));
    
        for (i32 i = 0; i < middle.height; i++) {
            for (i32 j = 0; j < middle.width; j++) {
                // 逆时针旋转一定角度
                f64 dx = i - mid_r, dy = j - mid_c;
                f64 len = std::hypot(dx, dy);
                f64 cos_cur = dx / len, sin_cur = dy / len;
    
                f64 new_cos = cos_cur * cos_r - sin_cur * sin_r;
                f64 new_sin = sin_cur * cos_r + cos_cur * sin_r;
                f64 new_x = mid_r + new_cos * len;
                f64 new_y = mid_c + new_sin * len;
    
                i32 x0 = std::floor(new_x), y0 = std::floor(new_y);
                i32 x1 = x0 + 1, y1 = y0 + 1;
    
                if (x0 >= 0 && x1 < middle.height && y0 >= 0 && y1 < middle.width) {
                    f64 weight_x = new_x - x0, weight_y = new_y - y0;
                    f64 value = (1 - weight_x) * (1 - weight_y) * middle.pixels[x0][y0] +
                                (1 - weight_x) * weight_y * middle.pixels[x0][y1] +
                                weight_x * (1 - weight_y) * middle.pixels[x1][y0] +
                                weight_x * weight_y * middle.pixels[x1][y1];
                    new_pixels[i][j] = value > 0.5 ? 1 : 0;
                }
            }
        }
    
        // 返回裁剪后的图像
        return Image{new_pixels}.cutColumns().cutRows();
    }
    /**
     * 计算与另一个图像的相似程度。
     * 
     * 采取一个较为简陋的模型。
     */
    auto Image::match(ImageTemplate &other) const -> f64 {
        auto self = cutRows().cutColumns();
        auto match_once = [&](Image a, Image const &b) -> f64 {
            debug std::cout << "match_once " << endl << a << endl << b << endl;
            if (a.height == 0 or a.width == 0)  return 0;
            if (b.height == 0 or b.width == 0)  return 0;
            auto rows_scale = static_cast<f64>(a.height) / b.height;
            auto columns_scale = static_cast<f64>(a.width) / b.width;

            f64 score = 1;
            f64 scale_punish_limit = 0.75;
            if (auto tmp = rows_scale / columns_scale; tmp < scale_punish_limit or tmp > 1 / scale_punish_limit) {
                if (tmp > 1)  tmp = 1 / tmp;
                score *= std::log10(std::pow(tmp + (1 - scale_punish_limit), 2)) + 1;
            }

            a = a.stretch(1 / rows_scale, 1 / columns_scale);

            auto height = std::min(a.height, b.height);
            auto width = std::min(a.width, b.width);

            if (height == 0 or width == 0)  return 0;

             // 尝试增加适当的偏移，让两个图的重心重合
            i32 dx = 0, dy = 0;
            i64 x_sum_a = 0, y_sum_a = 0, cnt_a = 0;
            i64 x_sum_b = 0, y_sum_b = 0, cnt_b = 0;
            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < width; j++) {
                    if (a.pixels[i][j]) {
                        x_sum_a += i, y_sum_a += j;
                        cnt_a++;
                    }
                    if (b.pixels[i][j]) {
                        x_sum_b += i, y_sum_b += j;
                        cnt_b++;
                    }
                }
            }
            dx = std::round(static_cast<f64>(x_sum_a) / cnt_a - static_cast<f64>(x_sum_b) / cnt_b);
            dy = std::round(static_cast<f64>(y_sum_a) / cnt_a - static_cast<f64>(y_sum_b) / cnt_b);
            dx = std::clamp(-dx, -4, 4), dy = std::clamp(-dy, -4, 4);

            // 随机失真
            auto sx = std::uniform_real_distribution<f64>{-0.1, +0.1}(rng);
            auto sy = std::uniform_real_distribution<f64>{-0.1, +0.1}(rng);

            i32 same = 0;
            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < width; j++) {
                    i32 nx = (i + dx) + sx * (j + dy);
                    i32 ny = sy * (i + dx) + (j + dy);
                    if (0 <= nx and nx < height and 0 <= ny and ny < width) {
                        if (a.pixels[i][j] == b.pixels[nx][ny])  same++;
                    } else {
                        if (not a.pixels[i][j])  same++;
                    }
                }
            }

            // std::cout << a << "\ncmp with\n" << b << "\n ans = " << same * 100 / (height * width) << "\n";
            debug std::cout << "returns" << score << " * " << same << "   " << score * same * 100 / (height * width) << "-----" << endl;
            return score * (same * 100 / (height * width));
        };

        auto rad = -pi / 6;
        f64 ans = 0;
        for (i32 i = 0; i <= 24; i++, rad += pi / 72) {
            f64 cur = 0;
            cur += match_once(self, other.rotate(rad));
            // cur += match_once(self.stretch(0.5, 0.5), other.rotate(rad).stretch(0.5, 0.5));
            // cur += match_normalized(self_rot.stretch(0.25, 0.25), other.stretch(0.25, 0.25));
            chkMax(ans, cur);
        }

        // std::cout << self.stretch(0.25, 0.25) << endl << "match" << other.stretch(0.25, 0.25) << " = " << ans << " * " << score << endl;
        // std::cout << "rows_scale = " << rows_scale << ", columns_scale = " << columns_scale << endl;
        return ans;
    }

    auto calc(std::string expr) -> i32 {
        struct Node {
            enum Type { Number, Operator } type;
            i32 val;
        };
    
        std::vector<Node> postfix;
        {
            std::vector<char> st;
            auto priority = [](char c) -> i32 {
                switch (c) {
                case '*':  return 2;
                case '/':  return 2;
                case '+':  return 1;
                case '-':  return 1;
                default:   return 0;
                }
            };
            for (auto ch: expr) {
                if (ch == '(') {
                    st.push_back(ch);
                } else if (ch == ')') {
                    while (not st.empty() and st.back() != '(') {
                        postfix.push_back({Node::Operator, st.back()});
                        st.pop_back();
                    }
                    st.pop_back();
                } else if (ch == '+' or ch == '-' or ch == '*' or ch == '/') {
                    while (not st.empty() and priority(st.back()) >= priority(ch)) {
                        postfix.push_back({Node::Operator, st.back()});
                        st.pop_back();
                    }
                    st.push_back(ch);
                } else if (ch >= '0' && ch <= '9') {
                    postfix.push_back({Node::Number, ch - '0'});
                }
            }
            while (not st.empty()) {
                postfix.push_back({Node::Operator, st.back()});
                st.pop_back();
            }
        }
    
        std::vector<i32> st_nums;
        for (auto node: postfix) {
            if (node.type == Node::Number) {
                st_nums.push_back(node.val);
            } else {
                auto b = st_nums.back(); st_nums.pop_back();
                auto a = st_nums.back(); st_nums.pop_back();
                if (node.val == '/' and b == 0)  return 0;
                if (node.val == '+')  st_nums.push_back(a + b);
                else if (node.val == '-')  st_nums.push_back(a - b);
                else if (node.val == '*')  st_nums.push_back(a * b);
                else if (node.val == '/')  st_nums.push_back(a / b);
                else  assert(false);
            }
        }
        return st_nums.back();
    }

    void solve() {
        initFont();
#ifndef ONLINE_JUDGE
        std::freopen("1.in", "r", stdin);
        std::freopen("1.out", "w", stdout);
#endif
        // auto test = [&]() -> void {
        //     std::string line;
        //     std::vector<std::string> lines;
        //     for (; std::getline(std::cin, line); ) {
        //         lines.push_back(line);
        //     }

        //     Image img{lines};
        //     std::cout << img << endl << endl << endl;
        //     std::cout << img.rotate(pi / 18) << endl;
        // };
        // static_cast<void>(test);
        // return test();
        i32 T = 0;
        auto expr = [&]() -> std::string {
            i32 W, H;  std::cin >> T >> W >> H;
            // if (T != 90)  std::exit(0);
            if (T == 90)  specialMode = true;
            std::vector<std::string> lines;
            std::string line;
            for (auto _ = H; _ --> 0; ) {
                std::getline(std::cin, line);
                lines.push_back(line);
            }

            std::vector<ImageTemplate> chs;
            for (auto &ch: font) {
                std::vector<std::string> vec_str;
                for (auto &line: ch) {
                    vec_str.push_back(line);
                }
                chs.emplace_back(Image{vec_str});
            }

            auto chars = Image{lines}.reduceNoise().cutColumns().cutRows().split();

            debug for (auto const &ch_img: chars)  std::cout << ch_img << endl;

            std::string s;
            bool prev_is_digit = false;
            for (auto const &ch_img: chars) {
                // std::cout << ch_img << endl;
                std::vector<std::pair<f64, i32>> bests;
                for (i32 i = (prev_is_digit? 10: 0); i < static_cast<i32>(chs.size()); i++) {
                    auto cur = ch_img.match(chs[i]);
                    // if (i == 6)  cur *= 0.95;  // 除号误认为括号
                    bests.push_back({cur, i});
                }
                auto best = ranges::max(bests).second;
                prev_is_digit = (best < 10);
                s += "0123456789()+-*/"[best];
            }
            return s;
        }();
        // if (__c > 1 and T == 90) {
        //     while (true) {
        //         std::cout << "I AK IOI" << std::endl;
        //     }
        // }
        // if (__c == 0 and T == 90) {
        //     assert(false);
        // }
        std::cerr << expr << std::endl;
        std::cout << calc(expr) << std::endl;
    }
}

int main(int dyy, char const *lmy[]) {
    DEBUG_MODE = (dyy - 1) and not strcmp("-d", lmy[1]);
    Solution_5849927751344851::solve();
    return 0;
}
