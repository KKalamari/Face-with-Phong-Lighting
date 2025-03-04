# **OpenGL Project - Phong Lighting & Moving Light Source**

This project was developed using **Visual Studio on Windows** and implements **Phong lighting** in an OpenGL environment. A sphere, representing a light source, moves in a circular orbit around a **3D face model**. The scene is fully interactive, allowing the user to control the **camera and lighting speed**.

---

## **📌 Features**
✔ **OpenGL initialization** with **GLFW** and **GLAD**  
✔ **Phong lighting model** with dynamic lighting  
✔ **3D models** loaded from `.obj` files (face and sphere)  
✔ **User-controlled camera movement** (WASD & mouse)  
✔ **Adjustable sphere rotation speed**  

---

## **🖥️ `main.cpp` - Core Program Functionality**
- Initializes **OpenGL**, **shaders**, and **3D models**.
- The **camera** can be moved by the user.
- The **sphere moves in a circular orbit** around the face model, with its position calculated using **sine and cosine functions**.
- The **rotation speed of the sphere** can be adjusted using the **"H" (increase) and "J" (decrease) keys**.
- The program **processes keyboard and mouse input**, allowing smooth **camera movement**.
- **Phong lighting model** is used, with the **light source coming from the sphere's position**.
- In the main **while loop**, the sphere's position is updated, transformations are applied to models, and everything is rendered.

---

## **💡 Shaders**
### ** `face.vs` (Vertex Shader - Face)**
Transforms the **local vertex coordinates** of the face model into **global coordinates** and passes data to the fragment shader:
- Applies transformations: `model`, `view`, and `projection`.
- Computes **normal vectors (`Normal`)** for lighting calculations.
- Passes **texture coordinates (`TexCoords`)** to the fragment shader.

### ** `face.fs` (Fragment Shader - Face)**
Implements **Phong lighting** and determines the **final color** of the face model:
- Computes **ambient, diffuse, and specular lighting**.
- Applies **light attenuation** (reduces brightness over distance).
- Combines all lighting components to determine the final **pixel color (`FragColor`)**.

### ** `sphere.vs` (Vertex Shader - Sphere)**
- Computes the **position of the sphere's vertices**.
- Applies transformations using `model`, `view`, and `projection`.

### ** `sphere.fs` (Fragment Shader - Sphere)**
- **Simply colors the sphere white** (`vec4(1.0)`) since it represents the **light source**.
- No lighting calculations are applied to the sphere itself.

---

## **🎮 Controls**
| Key | Action |
|-----|--------|
| **W, A, S, D** | Move the **camera** forward, left, backward, and right. |
| **Mouse** | Rotate the **camera**. |
| **H** | Increase **sphere rotation speed**. |
| **J** | Decrease **sphere rotation speed**. |
| **ESC** | Exit the program. |

---

## **🚀 How to Run**
1. **Clone the repository**:
   ```sh
   git clone https://github.com/your-username/your-repo.git
   cd your-repo
   ```
2. **Open the project in Visual Studio**.
3. **Ensure the necessary dependencies** (GLFW, GLAD, Assimp) are included.
4. **Build and Run** the project.

---

**Acknowledgments**

    [OpenGL - The Cherno - YouTube](https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&si=nHk_QFPstL1femtw)
    [LearnOpenGL - Basic Lighting](https://learnopengl.com/Lighting/Basic-Lighting)

