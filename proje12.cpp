#include <curl/curl.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//fopen hatasýný çozer
#pragma warning (disable : 4996)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//glew glfw3 den once olmasi lazim
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
int nokta(int x, int y, int position[100], int d) {
	int control = 0;
	int bak1 = 0;
	int bak2 = 0;
	for (int j = 0; j < d; j++)
	{
		int x1 = position[j * 2];
		int y1 = position[j * 2 + 1];
		int nextIndex = (j + 1) % d;
		int x2 = position[nextIndex * 2];
		int y2 = position[nextIndex * 2 + 1];
		//interpolation formula

		//þekilin saðýndan ekranýn en soluna doðru alýr
		if (y1 != y2) {
			if ((y1 <= y && y <= y2 || y2 <= y && y <= y1) && (x <= x1 + (x2 - x1) * (y - y1) / (y2 - y1)))
				bak1 = 1;
		}
		else {
			if ((y1 <= y && y < y2 || y2 <= y && y < y1) && (x <= x1 + (x2 - x1) * (y - y1) / (y2 - y1)))
				bak1 = 1;
		}



		//þeklin solundan ekranýn en saðýna doðru alýr
		if (y1 != y2) {
			if ((y1 <= y && y <= y2 || y2 <= y && y <= y1) && (x >= x1 + (x2 - x1) * (y - y1) / (y2 - y1)))
				bak2 = 1;
		}
		else {
			if ((y1 <= y && y < y2 || y2 <= y && y < y1) && (x >= x1 + (x2 - x1) * (y - y1) / (y2 - y1)))
				bak2 = 1;
		}
	}
	//ikisinin kesiþimi bize þekli verir
	if (bak1 == 1 && bak2 == 1) control = 1;
	return control;
}
int kareislem(int position[100], int d, int isAlan) {

	//4 tane nokta alýyoruz 
	int kenar1 = 0; int kenar2 = 0; int kenar3 = 0; int kenar4 = 0;
	int alan = 0;
	int karesayisi = 0;
	int sonuc = 0;
	//Karelere ayýrdýðýmýz alanlara bakar
	int bak[61][61] = { 0 };
	//Karelerin 1x1,2x2,4x4 yan yana olma durumlarýna bakar
	int bak2[61][61] = { 0 };

	int a = 0; int b = 0;
	int kontrol = 1;
	for (int x = 0; x <= 60; x++) {

		for (int y = 0; y <= 60; y++) {

			//karelerin konduðu  deðiyorsa kontrol 0
			kontrol = 1;
			for (int i = x; i <= x + 3; i++) {
				for (int j = y; j <= y + 3; j++) {
					if (bak[i][j] == 1) kontrol = 0;
				}
			}

			if (kontrol == 1 && (bak2[a][b] == 4 || bak2[a][b] == 2 || bak2[a][b] == 0)
				) {

				kenar1 = nokta(x, y, position, d);
				kenar2 = nokta(x + 3, y, position, d);
				kenar3 = nokta(x, y + 3, position, d);
				kenar4 = nokta(x + 3, y + 3, position, d);

				if ((kenar1 && kenar2 && kenar3 && kenar4) == 1) {

					alan += 16;
					karesayisi++;

					for (int i = x; i <= x + 3; i++) {
						for (int j = y; j <= y + 3; j++) {
							bak[i][j] = 1;
							bak2[i][j] = 4;

						}
					}
				}
			}

			kontrol = 1;

			for (int i = x; i <= x + 1; i++) {
				for (int j = y; j <= y + 1; j++) {
					if (bak[i][j] == 1) kontrol = 0;


				}
			}

			if (kontrol == 1) {
				kenar1 = nokta(x, y, position, d);
				kenar2 = nokta(x + 1, y, position, d);
				kenar3 = nokta(x, y + 1, position, d);
				kenar4 = nokta(x + 1, y + 1, position, d);
				if ((kenar1 && kenar2 && kenar3 && kenar4) == 1)
				{
					alan += 4;
					karesayisi++;
					for (int i = x; i <= x + 1; i++) {
						for (int j = y; j <= y + 1; j++) {
							bak[i][j] = 1;
							bak2[i][j] = 2;


						}
					}
				}
			}

			if (bak[x][y] != 1 && (bak2[a][b] == 2 || bak2[a][b] == 1 || bak2[a][b] == 0)
				) {

				kenar1 = nokta(x, y, position, d);

				if ((kenar1) == 1)
				{
					alan += 1;
					karesayisi++;
					bak[x][y] = 1;
					bak2[x][y] = 1;

				}
			}
			//oluþturulan karelerin olduðu sütuna götürür
			b = y;
		}
		//oluþturulan karelerin olduðu satýra götürür
		a = x;
	}
	//alan almak için 1 kare sayisi almak için 0
	if (isAlan == 1)
		sonuc = alan;

	else
		sonuc = karesayisi;

	for (int i = 0; i < 61; i++) {
		for (int j = 0; j < 61; j++) {
			bak[i][j] = 0;
			bak2[i][j] = 0;
		}
	}
	return sonuc;
}

int raster(int x, int y, int position[100], int d, int positional[8], int bak2[61][61]) {

	int kenar1 = 0; int kenar2 = 0; int kenar3 = 0; int kenar4 = 0;
	int renk = 0;
	int bak[61][61];
	int a = 0; int b = 0;
	int kontrol = 1;
	b = y;

	if (x != 0) a = x - 1;
	else a = x;

	for (int i = x; i <= x + 3; i++) {
		for (int j = y; j <= y + 3; j++) {
			if (bak[i][j] == 1) kontrol = 0;
		}
	}

	if (kontrol == 1 && (bak2[a][b] == 4 || bak2[a][b] == 2 || bak2[a][b] == 0)
		) {
		kenar1 = nokta(x, y, position, d);
		kenar2 = nokta(x + 3, y, position, d);
		kenar3 = nokta(x, y + 3, position, d);
		kenar4 = nokta(x + 3, y + 3, position, d);

		if ((kenar1 && kenar2 && kenar3 && kenar4) == 1) {

			for (int i = x; i <= x + 3; i++) {
				for (int j = y; j <= y + 3; j++) {
					bak[i][j] = 1;
					bak2[i][j] = 4;
				}
			}
			renk = 4;
			positional[0] = x;
			positional[1] = y;
			positional[2] = x + 4;
			positional[3] = y;
			positional[4] = x + 4;
			positional[5] = y + 4;
			positional[6] = x;
			positional[7] = y + 4;
		}
	}

	kontrol = 1;

	for (int i = x; i <= x + 1; i++) {
		for (int j = y; j <= y + 1; j++) {
			if (bak[i][j] == 1) kontrol = 0;
		}
	}

	if (kontrol == 1) {
		kenar1 = nokta(x, y, position, d);
		kenar2 = nokta(x + 1, y, position, d);
		kenar3 = nokta(x, y + 1, position, d);
		kenar4 = nokta(x + 1, y + 1, position, d);
		if ((kenar1 && kenar2 && kenar3 && kenar4) == 1)
		{

			for (int i = x; i <= x + 1; i++) {
				for (int j = y; j <= y + 1; j++) {
					bak[i][j] = 1;
					bak2[i][j] = 2;
				}
			}

			renk = 2;
			positional[0] = x;
			positional[1] = y;
			positional[2] = x + 2;
			positional[3] = y;
			positional[4] = x + 2;
			positional[5] = y + 2;
			positional[6] = x;
			positional[7] = y + 2;
		}
	}
	kontrol = 1;

	for (int i = x; i <= x ; i++) {
		for (int j = y; j <= y ; j++) {
			if (bak[i][j] == 1) kontrol = 0;
		}
	}

	if (kontrol==1 && (bak2[a][b] == 2 || bak2[a][b] == 1 || bak2[a][b] == 0)
		) {

		kenar1 = nokta(x, y, position, d);
		if ((kenar1) == 1)
		{

			bak[x][y] = 1;
			bak2[x][y] = 1;

			renk = 1;
			positional[0] = x;
			positional[1] = y;
			positional[2] = x + 1;
			positional[3] = y;
			positional[4] = x + 1;
			positional[5] = y + 1;
			positional[6] = x;
			positional[7] = y + 1;

		}
	}

	return renk;
}

size_t yazdir(void* data, size_t boyut, size_t nmemb, void* gelendata);

//Shoelace Area Formula 
float AlanHesap(int position[100], int d) {

	float alan = 0;
	float toplam1 = 0;
	float toplam2 = 0;
	//Area = 1 / 2??(x1?y2? + x2?y3? + … + xn?1?yn? + xn?y1?)?(x2?y1? + x3?y2? + … + xn?yn?1? + x1?yn?)?
	for (int i = 0; i < d; i++)
	{
		if (i != d - 1) {
			toplam1 += position[i * 2] * position[i * 2 + 3];
			toplam2 += position[i * 2 + 2] * position[i * 2 + 1];
		}
		else {
			toplam1 += position[i * 2] * position[1];
			toplam2 += position[0] * position[i * 2 + 1];
		}
	}
	//Shoelace Area Formula
	alan = fabs(toplam1 - toplam2) * 0.5;
	return alan;
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //&source[0]
	//shader ,ne kadar, source kod,
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

//isimlendirme
typedef struct {
	//urlden degeri string alan
	char* string;

	size_t boyut;

}Yanit;

size_t yazdir(void* data, size_t boyut, size_t nmemb, void* gelendata);

int main() {
	
	//urlye ulasmak icin
	CURL* curl;

	//urlnin degeri
	CURLcode sonuc;

	//url deger verdi mi vermedi mi
	curl = curl_easy_init();

	if (curl == NULL)
	{
		printf("Baglanti saglanamadi !\n");

		return -1;

	}

	Yanit yanit;
	yanit.string = (char*)malloc(1);
	yanit.boyut = 0;

	//urlden deger almak icin
	curl_easy_setopt(curl, CURLOPT_URL, "http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt");

	//urlyi stringe cevirme
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, yazdir);
	//
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&yanit);

	sonuc = curl_easy_perform(curl);

	//baglantiyi sonlandir
	curl_easy_cleanup(curl);
	
	FILE* dosya;

	dosya = fopen("test.txt", "w");
	//Webden aldýðýmýz Noktalarý dosyaya koy
	fputs(yanit.string, dosya);
	fclose(dosya);

	char nok[100][1000];

	int satir = 0;

	dosya = fopen("test.txt", "r");

	//dosyanin sonuna ulasana kadar
	while (!feof(dosya))
	{
		//Noktalarý satir satir al
		if (fgets(nok[satir], 1000, dosya) != 0)
			satir++;
	}

	fclose(dosya);

	int secim = 0;
	printf("Hangi satiri cizdirmek istersiniz\n");
	
	scanf("%d", &secim);

	char* ARA;
	ARA = strtok(nok[secim - 1], "()");

	char noktalar[100][500];
	int Sira = 0;

	while (ARA != NULL) {

		if (strcmp(ARA, "1B") != 0 && strcmp(ARA, "2B") != 0 && strcmp(ARA, "3B") != 0 && strcmp(ARA, "F") != 0 && (strlen(ARA)) > 1) {

			strcpy(noktalar[Sira], ARA);
			Sira++;
		}

		ARA = strtok(NULL, "()");
	}

	int a, b, c;
	a = 0;
	b = 0;
	c = 0;
	unsigned int var = 0;

	int x, y;
	int Al_i = 0;
	int Al_i2 = 0;

	//20,20 ,30,20 ,20,40 ,10,40
	int positions1[100]{};
	int positions2[100]{};
	int positions3[100]{};
	int positions4[8] = {};
	//1.sira 
	for (int i = 1; i < Sira; i++) {
		if (strcmp(noktalar[0], noktalar[i]) == 0) {
			//1.satýrýn sonu
			Al_i = i + 1;
			break;
		}
	}
	//2. sira varsa
	for (int i = Al_i + 1; i < Sira; i++) {
		if (strcmp(noktalar[Al_i], noktalar[i]) == 0) {
			//2.satýrýn sonu
			Al_i2 = i + 1;
			var = 1;
			break;
		}
	}
	//3. sira varsa
	if (var == 1) {
		for (int i = Al_i2 + 1; i < Sira; i++) {
			if (strcmp(noktalar[Al_i2], noktalar[i]) == 0) {

				var = 2;
				break;
			}
		}
	}
	//1.sira
	for (int i = 0; i < Al_i; i++) {
		sscanf(noktalar[i], "%d,%d", &x, &y);

		positions1[i * 2] = x;
		positions1[i * 2 + 1] = y;
		a++;
	}

	//2.sira
	if (var == 2) {
		for (int i = Al_i; i < Sira - Al_i; i++) {
			sscanf(noktalar[i], "%d,%d", &x, &y);

			positions2[(i - Al_i) * 2] = x;
			positions2[(i - Al_i) * 2 + 1] = y;
			b++;
		}
	}

	else {
		for (int i = Al_i; i < Sira; i++) {
			sscanf(noktalar[i], "%d,%d", &x, &y);

			positions2[(i - Al_i) * 2] = x;
			positions2[(i - Al_i) * 2 + 1] = y;
			b++;
		}
	}



	if (var == 2) {
		//3.sira
		for (int i = Al_i2; i < Sira; i++) {
			sscanf(noktalar[i], "%d,%d", &x, &y);

			positions3[(i - Al_i2) * 2] = x;
			positions3[(i - Al_i2) * 2 + 1] = y;
			c++;
		}
	}

	if (a >= 1) a--;
	if (b >= 1) b--;
	if (c >= 1) c--;
	
	float Alan;
	Alan = AlanHesap(positions1, a);
	if (b >= 3)
		Alan += AlanHesap(positions2, b);
	if (c >= 3)
		Alan += AlanHesap(positions3, c);

	float Rezervdeger = 0;
	Rezervdeger = Alan * 10;

	printf("toplam rezerv degeri = %f\n", Rezervdeger);

	//alan almak için 1 kare sayisi almak için 0
	int gelenkare1 = kareislem(positions1, a, 0);
	int gelenalan1 = kareislem(positions1, a, 1);
	int gelenkare2 = kareislem(positions2, b, 0);
	int gelenalan2 = kareislem(positions2, b, 1);
	int gelenkare3 = kareislem(positions3, c, 0);
	int gelenalan3 = kareislem(positions3, c, 1);


	int toplamgelenalan = gelenalan1 + gelenalan2 + gelenalan3;
	int toplramgelenkare = gelenkare1 + gelenkare2 + gelenkare3;

	printf("toplam kareler=%d \n", toplramgelenkare);
	printf("toplam alan=%d \n", toplamgelenalan);


	GLFWwindow* window;

	// Initialize the library 
	if (!glfwInit())
		return -1;


	//pencere oluþturma
	window = glfwCreateWindow(960, 540, "ALAN", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Glew  ve GLFW nin birlikte calýþsmasý için gerekli kod
	if (glewInit() != GLEW_OK)
		std::cout << " Error! " << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions1), positions1, GL_STATIC_DRAW);
	//kacýncý vertex etkinleþtir
	glEnableVertexAttribArray(0);
	//kacýncý vertex / kaç  kordinat / int / normalize/1 sýrada kaç byte var/
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(int) * 2, 0);

	unsigned int vao2;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);

	unsigned int buffer2;
	glGenBuffers(1, &buffer2);
	glBindBuffer(GL_ARRAY_BUFFER, buffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions2), positions2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(int) * 2, 0);

	unsigned int vao3;
	glGenVertexArrays(1, &vao3);
	glBindVertexArray(vao3);

	unsigned int buffer3;
	glGenBuffers(1, &buffer3);
	glBindBuffer(GL_ARRAY_BUFFER, buffer3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions3), positions3, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(int) * 2, 0);


	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location =0) in vec4 position;"
		"\n"
		"uniform mat4 transform;"
		"\n"
		"void main()\n"
		"{\n"
		"gl_Position=transform * position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		"color= vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	//Þekillerin ekranýn sað altýnda oluþmasý için glm kullandým
	glm::mat4 orthoProjection = glm::ortho(0.0f, 60.0f, 60.0f, 0.0f, -1.0f, 1.0f);

	unsigned int transformLoc = glGetUniformLocation(shader, "transform");
	if (transformLoc == -1) {
		std::cout << "Error: Uniform location not found." << std::endl;
	}

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(orthoProjection));


	int kontrol = 0;

	//Pencere kapanana kadar çalýþtýr
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_POLYGON, 0, a);

		glBindVertexArray(vao2);
		glDrawArrays(GL_POLYGON, 0, b);

		glBindVertexArray(vao3);
		glDrawArrays(GL_POLYGON, 0, c);

		glfwSwapBuffers(window);
		
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	float birimsondajmaliyeti = 0;
	float birimplatformmaliyeti = 0;

	while (!(birimsondajmaliyeti >= 1 && birimsondajmaliyeti <= 10)) {
		printf("Lutfen 1-10 arasi birim sondaj maliyeti girin: \n");
		scanf("%f", &birimsondajmaliyeti);
	}
	printf("Lutfen birim platform maliyeti girin: \n");
	scanf("%f", &birimplatformmaliyeti);

	float masrafalan = toplamgelenalan * birimsondajmaliyeti;
	float masrafkare = toplramgelenkare * birimplatformmaliyeti;

	GLFWwindow* window2;

	window2 = glfwCreateWindow(960, 540, "ALAN2", NULL, NULL);
	if (!window2)
	{
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK)
		std::cout << " Error! " << std::endl;

	glfwMakeContextCurrent(window2);

	unsigned int vao4;
	glGenVertexArrays(1, &vao4);
	glBindVertexArray(vao4);

	unsigned int buffer4;
	glGenBuffers(1, &buffer4);
	glBindBuffer(GL_ARRAY_BUFFER, buffer4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions4), positions4, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(int) * 2, 0);

	std::string vertexShader2 =
		"#version 330 core\n"
		"\n"
		"layout(location =0) in vec4 position;"
		"\n"
		"uniform mat4 transform;"
		"\n"
		"uniform vec4 vertexColor;"
		"\n"
		"void main()\n"
		"{\n"
		"gl_Position=transform * position;\n"
		"}\n";

	std::string fragmentShader2 =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"
		"\n"
		"uniform vec4 vertexColor;"
		"\n"
		"void main()\n"
		"{\n"
		"color=vertexColor;\n"
		"}\n";

	unsigned int shader2 = CreateShader(vertexShader2, fragmentShader2);
	glUseProgram(shader2);

	//ikinci pencere için uniformlarý güncelliyorum

	glm::mat4 orthoProjection2 = glm::ortho(0.0f, 60.0f, 60.0f, 0.0f, -1.0f, 1.0f);
	unsigned int transformLoc2 = glGetUniformLocation(shader2, "transform");
	if (transformLoc2 == -1) {
		std::cout << "Error: Uniform location not found." << std::endl;
	}
	glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(orthoProjection2));

	glm::vec4 vertexColor(1.0f, 0.0f, 0.0f, 1.0f);

	int vertexColorLocation = glGetUniformLocation(shader2, "vertexColor");
	int bak1[61][61] = { 0 };
	int bak2[61][61] = { 0 };
	int bak3[61][61] = { 0 };
	
	while (!glfwWindowShouldClose(window2))
	{
		glUseProgram(shader2);

		glClear(GL_COLOR_BUFFER_BIT);

		for (x = 0; x <= 60; x++) {
			for (y = 0; y <= 60; y++) {

				kontrol = raster(x, y, positions1, a, positions4, bak1);

				if (kontrol != 0) {
					if (kontrol == 4) {
						glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
					}
					else if (kontrol == 2) {
						glUniform4f(vertexColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
					}
					else
						glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);

					glBindVertexArray(vao4);
					glBufferData(GL_ARRAY_BUFFER, sizeof(positions4), positions4, GL_STATIC_DRAW);
					glDrawArrays(GL_POLYGON, 0, 4);
				}

				kontrol = raster(x, y, positions2, b, positions4, bak2);
				if (kontrol != 0) {
					if (kontrol == 4) {
						glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
					}
					else if (kontrol == 2) {
						glUniform4f(vertexColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
					}
					else
						glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);

					glBindVertexArray(vao4);
					glBufferData(GL_ARRAY_BUFFER, sizeof(positions4), positions4, GL_STATIC_DRAW);
					glDrawArrays(GL_POLYGON, 0, 4);

				}
				kontrol = raster(x, y, positions3, c, positions4, bak3);
				if (kontrol != 0) {
					if (kontrol == 4) {
						glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
					}
					else if (kontrol == 2) {
						glUniform4f(vertexColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
					}
					else
						glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);


					glBindVertexArray(vao4);
					glBufferData(GL_ARRAY_BUFFER, sizeof(positions4), positions4, GL_STATIC_DRAW);
					glDrawArrays(GL_POLYGON, 0, 4);

				}


			}
		}

		glfwSwapBuffers(window2);
		glfwPollEvents();
	}
	glDeleteProgram(shader2);

	glfwTerminate();

	//Toplam kar 
	float kar = Rezervdeger - masrafalan - masrafkare;
	if (kar > 0)
		printf("Edilen kar =%f", kar);
	else
		printf("Edilen en az zarar=%f", kar);

	free(yanit.string);
	return 0;
}

size_t yazdir(void* data, size_t boyut, size_t nmemb, void* gelendata) {


	//size 1 ve nmemb alianan deger sayisi
	size_t real_size = boyut * nmemb;

	Yanit* yanit = (Yanit*)gelendata;
	//char* olarak almak için realloc kullandýk
	char* ptr = (char*)realloc(yanit->string, yanit->boyut + real_size + 1);

	yanit->string = ptr;
	memcpy(&(yanit->string)[yanit->boyut], data, real_size);
	yanit->boyut += real_size;
	yanit->string[yanit->boyut] = '\0';
	return real_size;
}