#include"SVGTransform.h"
#include<sstream>
#include<cctype>
#include<algorithm>


static vector<float>parseNumbers(const string& s)
{
	vector<float> nums;
	stringstream ss(s);
	string token;

	char c;
	string cur;
	for (char ch : s) {
		if (ch == ',' || isspace((unsigned char)ch)) {
			if (!cur.empty()) {
				nums.push_back(stof(cur));
				cur.clear();
			}
		}
		else {
			cur.push_back(ch);
		}
	}

	if (!cur.empty()) {
		nums.push_back(stof(cur));
	}
	return nums;
}


SVGTransform SVGTransform::parse(const string& str)
{
	SVGTransform t;
	string s = str;

	s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
		}));

	s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());

	size_t pos = 0;
	while (pos < s.size()) {
		if (s.compare(pos, 9, "translate") == 0) {
			pos += 9;
			size_t l = s.find('(', pos);
			size_t r = s.find(')', l + 1);
			if (l == string::npos || r == string::npos) break;

			string inside = s.substr(l + 1, r - l - 1);
			auto nums = parseNumbers(inside);
			float tx = nums.size() > 0 ? nums[0] : 0.0f;
			float ty = nums.size() > 1 ? nums[1] : 0.0f;
			t.addTranslate(tx, ty);
			pos = r + 1;
		}
		else if (s.compare(pos, 6, "rotate") == 0) {
			pos += 6;
			size_t l = s.find('(', pos);
			size_t r = s.find(')', l + 1);
			if (l == string::npos || r == string::npos) break;

			string inside = s.substr(l + 1, r - l - 1);
			auto nums = parseNumbers(inside);
			float deg = nums.size() > 0 ? nums[0] : 0.0f;
			t.addRotate(deg);
			pos = r + 1;
		}
		else if (s.compare(pos, 5, "scale") == 0) {
			pos += 5;
			size_t l = s.find('(', pos);
			size_t r = s.find(')', l + 1);
			if (l == string::npos || r == string::npos) break;

			string inside = s.substr(l + 1, r - l - 1);
			auto nums = parseNumbers(inside);
			float sx = nums.size() > 0 ? nums[0] : 1.0f;
			float sy = nums.size() > 1 ? nums[1] : sx;
			t.addScale(sx, sy);
			pos = r + 1;
		}
		else {
			++pos;
		}
	}

	return t;
}

void SVGTransform::applyToMatrix(Gdiplus::Matrix& m) const
{
	for (const auto& x : ops) {
		switch (x.type)
		{
		case TransformType::Translate:
			m.Translate(x.a, x.b);
			break;
		case TransformType::Rotate:
			m.Rotate(x.a);
			break;
		case TransformType::Scale:
			m.Scale(x.a, x.b);
			break;
		}
	}
}