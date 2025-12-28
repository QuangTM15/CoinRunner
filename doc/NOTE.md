1️⃣ sf::Text KHÔNG CÓ DEFAULT CONSTRUCTOR

❌ Sai (SFML 2 quen tay):

sf::Text text;


✅ Đúng (SFML 3):

sf::Text text(font, "PLAY", 16);


hoặc (khi dùng std::optional):

std::optional<sf::Text> text;
text.emplace(font, "PLAY", 16);


📌 Hệ quả

struct chứa sf::Text → constructor mặc định bị xoá

Phải dùng std::optional<sf::Text> hoặc std::unique_ptr

2️⃣ sf::Sprite CŨNG KHÔNG CÓ DEFAULT CONSTRUCTOR

❌ Sai:

sf::Sprite sprite;


✅ Đúng:

sf::Sprite sprite(texture);


hoặc:

std::optional<sf::Sprite> sprite;
sprite.emplace(texture);


📌 Rule chốt

Trong SFML 3:
Text / Sprite / Shape → nên dùng optional nếu khởi tạo trễ

3️⃣ std::optional<T> – CÚ PHÁP ĐÚNG

❌ Sai:

item.text.setString("PLAY");


❌ Sai:

if (item.text) item.text.setString("PLAY");


✅ Đúng:

if (item.text)
    item.text->setString("PLAY");


✅ Hoặc:

auto& t = *item.text;
t.setString("PLAY");


📌 Không bao giờ

!item.text

*item.text khi chưa check

4️⃣ sf::Font dùng openFromFile (KHÔNG phải loadFromFile)

❌ Sai (SFML 2):

font.loadFromFile("pixel.ttf");


✅ Đúng (SFML 3):

font.openFromFile("pixel.ttf");


📌 Nếu font không load:

Text hiện ô vuông / dấu chấm

Không crash → rất dễ nhầm là bug text

5️⃣ Text hiện “dấu chấm / ô vuông” – NGUYÊN NHÂN THẬT

❌ Không phải do:

scale

characterSize

string

✅ Thường do:

Sai đường dẫn font

Font không có glyph ASCII

Font load fail nhưng không check return

✅ Cách an toàn:

if (!font.openFromFile("asset/ui/fonts/pixel.ttf"))
    std::cout << "Font load FAILED\n";

6️⃣ sf::Text + center = BẮT BUỘC setOrigin bằng bounds

❌ Sai (text lệch):

text.setPosition(cx, cy);


✅ Đúng:

auto b = text.getLocalBounds();
text.setOrigin(
    b.position.x + b.size.x * 0.5f,
    b.position.y + b.size.y * 0.5f
);
text.setPosition(cx, cy);


📌 Luôn gọi sau khi

setString

setCharacterSize

setScale

7️⃣ Sprite button dài / dọc → SCALE KHÔNG PHẢI VUÔNG

❌ Sai:

sprite.setScale(4.f, 4.f);


✅ Đúng:

sprite.setScale(12.f, 4.f); // kéo ngang


📌 Texture vuông ≠ Button vuông
👉 Scale tự do theo UX

8️⃣ Texture / Font / SoundBuffer
⚠️ PHẢI SỐNG LÂU HƠN Object DÙNG NÓ

❌ Sai:

sf::Texture tex;
sprite.setTexture(tex); // tex sắp out scope


✅ Đúng:

Texture là member của MenuItem / Scene

Không tạo local rồi gán

9️⃣ Click menu = LOAD SCENE NGAY

→ KHÔNG CẦN pressed animation

📌 Thực tế UX:

Hover là đủ

Click là chuyển scene ngay

Pressed chỉ hợp khi có delay

👉 Quyết định đúng: bỏ pressed

🔒 TÓM TẮT LUẬT VÀNG (LOCK)

SFML 3 ≠ SFML 2 (rất nhiều constructor đổi)

sf::Text, sf::Sprite → không default ctor

Dùng std::optional cho UI element

Font luôn openFromFile

Text muốn căn giữa → dùng getLocalBounds

Texture / Font sống lâu hơn object

UI đơn giản > animation thừa