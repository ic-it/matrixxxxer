#include "array"
#include "vector"
#include "random"
#include "unistd.h"
#include "iostream"
#include "algorithm"
#include "SFML/Graphics.hpp"




int drawFrame(std::vector<int>& posOfParts, sf::Vector2i matrixSize, sf::Sprite& sprite, sf::Vector2i* parts, sf::Vector2i boxSize, sf::RenderWindow& window, sf::Vector2i indent)
{
    int i = 0;
    int j = 0;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return -1;
        }
    }

    window.clear(sf::Color::Black);
    for (std::vector<int>::iterator drawIdL=posOfParts.begin(); drawIdL!=posOfParts.end(); ++drawIdL)
    {
        if (j >= matrixSize.x)
        {
            j = 0;
            i++;
        }
        int drawId = *drawIdL;
        sprite.setTextureRect(sf::IntRect(parts[drawId].x, parts[drawId].y, boxSize.x, boxSize.y));
        sprite.setPosition(j * boxSize.x + j * indent.x, i * boxSize.y + i * indent.y);
        window.draw(sprite);
        j++;
    }
    window.display();
}


int MergeSortImpl(std::vector<int>& values, std::vector<int>& buffer, int l, int r, std::vector<int>& posOfParts, sf::Vector2i matrixSize, sf::Sprite& sprite, sf::Vector2i* parts, sf::Vector2i boxSize, sf::RenderWindow& window, sf::Vector2i indent)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        if (MergeSortImpl(values, buffer, l, m, posOfParts,matrixSize, sprite, parts, boxSize, window, indent) == -1)
        {
            return -1;
        }
        if (MergeSortImpl(values, buffer, m + 1, r, posOfParts,matrixSize, sprite, parts, boxSize, window, indent) == -1)
        {
            return -1;
        }

        int k = l;
        for (int i = l, j = m + 1; i <= m || j <= r; ) 
        {
            if (j > r || (i <= m && values[i] < values[j])) 
            {
                buffer[k] = values[i];
                ++i;
            } else 
            {
                buffer[k] = values[j];
                ++j;
            }
            ++k;
        }
        for (int i = l; i <= r; ++i) 
        {
            values[i] = buffer[i];
        }
        if (drawFrame(posOfParts,matrixSize, sprite, parts, boxSize, window, indent) == -1)
        {
            return -1;
        }
    }
}


int quickSort(std::vector<int>& posOfParts, sf::Vector2i matrixSize, sf::Sprite& sprite, sf::Vector2i* parts, sf::Vector2i boxSize, sf::RenderWindow& window, int low, int high, sf::Vector2i indent)
{
    int i = low;
    int j = high;
    int pivot = posOfParts[(i + j) / 2];
    int temp;

    while (i <= j)
    {
        while (posOfParts[i] < pivot)
            i++;
        while (posOfParts[j] > pivot)
            j--;
        if (i <= j)
        {
            temp = posOfParts[i];
            posOfParts[i] = posOfParts[j];
            posOfParts[j] = temp;
            i++;
            j--;
            if (drawFrame(posOfParts,matrixSize, sprite, parts, boxSize, window, indent) == -1)
            {
                return -1;
            }
        }
    }
    if (j > low)
        quickSort(posOfParts,matrixSize, sprite, parts, boxSize, window, low, j, indent);
    if (i < high)
        quickSort(posOfParts,matrixSize, sprite, parts, boxSize, window, i, high, indent);
}

void MergeSort(std::vector<int>& posOfParts, sf::Vector2i matrixSize, sf::Sprite& sprite, sf::Vector2i* parts, sf::Vector2i boxSize, sf::RenderWindow& window, sf::Vector2i indent) 
{
    std::vector<int> buffer(posOfParts.size());
    MergeSortImpl(posOfParts, buffer, 0, posOfParts.size() - 1, posOfParts,matrixSize, sprite, parts, boxSize, window, indent);
}


int HeapSort(std::vector<int>& posOfParts, sf::Vector2i matrixSize, sf::Sprite& sprite, sf::Vector2i* parts, sf::Vector2i boxSize, sf::RenderWindow& window, sf::Vector2i indent) 
{
    std::make_heap(posOfParts.begin(), posOfParts.end());
    for (auto i = posOfParts.end(); i != posOfParts.begin(); --i) {
        std::pop_heap(posOfParts.begin(), i);
        if (drawFrame(posOfParts, matrixSize, sprite, parts, boxSize, window, indent) == -1)
        {
            return -1;
        }
    }
}


int main()
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2i matrixSize = {70, 45};
    sf::Vector2i boxSize    = {20, 20};
    sf::Vector2i indent     = {0, 0};
    sf::Vector2i parts[matrixSize.x*matrixSize.y]; // positions
    std::vector<int> posOfParts;
    int sortedStep = 0;
    int sortType = 0;
    bool done = false;

    std::string windowTitle;
    std::string windowTitles[] = {
        "Matrixxxxer (Bouble)",
        "Matrixxxxer (Insert)",
        "Matrixxxxer (Selection)",
        "Matrixxxxer (Merge)",
        "Matrixxxxer (Quick)",
        "Matrixxxxer (Heap)"
    };

    if (!texture.loadFromFile("res/texture.jpg"))
    {
        std::cout << "TEXTRE NOT FOND" << std::endl; 
        return -1;
    }
    std::cout << "Sortings: " << std::endl;
    std::cout << "\t0. Bouble" << std::endl;
    std::cout << "\t1. Insert" << std::endl;
    std::cout << "\t2. Selection" << std::endl;
    std::cout << "\t3. Merge" << std::endl;  
    std::cout << "\t4. Quick" << std::endl;  
    std::cout << "\t5. Heap" << std::endl;  
    std::cout << "Sort Type: "; std::cin >> sortType;

    for (int i = 0; i < matrixSize.y; i++)
    {
        for (int j = 0; j < matrixSize.x; j++)
        {
            parts[i*matrixSize.x + j].x = j * boxSize.x + j * indent.x;
            parts[i*matrixSize.x + j].y = i * boxSize.y + i * indent.y;
            
            posOfParts.push_back(i*matrixSize.x + j);
        }
    }
    std::random_shuffle(posOfParts.begin(), posOfParts.end());
    sf::RenderWindow window(
        sf::VideoMode(
            matrixSize.x * boxSize.x + matrixSize.x*indent.x,
            matrixSize.y * boxSize.y + matrixSize.y*indent.y
        ), 
        windowTitles[sortType]);
    
    
    window.setFramerateLimit(60);
    sprite.setTexture(texture);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        if (sortedStep < posOfParts.size() && sortType < 3)
        {
            // пузырёк
            if (sortType == 0)
            {
                for (size_t idx_j = 0; idx_j + 1 < posOfParts.size() - sortedStep; ++idx_j) {
                    if (posOfParts[idx_j + 1] < posOfParts[idx_j]) {
                        std::swap(posOfParts[idx_j], posOfParts[idx_j + 1]);
                    }
                }
            }

            // вствавки
            if (sortType == 1)
            {
                int x = posOfParts[sortedStep];
                size_t t = sortedStep;
                while (t > 0 && posOfParts[t - 1] > x) {
                    posOfParts[t] = posOfParts[t - 1];
                    --t;
                }
                posOfParts[t] = x;
            }

            //Выбор
            if (sortType == 2)
            {
                auto i = posOfParts.begin() + sortedStep;
                auto j = std::min_element(i, posOfParts.end());
                std::swap(*i, *j);
            }

            sortedStep++;
            drawFrame(posOfParts,matrixSize, sprite, parts, boxSize, window, indent);
        }
        else if (!done && sortType == 3)
        {
            MergeSort(posOfParts, matrixSize, sprite, parts, boxSize, window, indent);
            done = true;
        }
        else if (!done && sortType == 4)
        {
            quickSort(posOfParts, matrixSize, sprite, parts, boxSize, window, 0, static_cast<int>(posOfParts.size())-1, indent);
            done = true;
        }
        else if (!done && sortType == 5)
        {
            HeapSort(posOfParts, matrixSize, sprite, parts, boxSize, window, indent);
            done = true;
        }
        else
        {
            sleep(1);
            continue;
        }

        
    }

    return EXIT_SUCCESS;
}
