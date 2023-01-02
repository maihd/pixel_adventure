struct UIMainScreen
{
    bool showDebug = false;

    void Render()
    {
        UIButton()
            .Title("Increment")
            .Position(vec2_new(0.0f, 0.0f))
            .Color(vec4_new(0.0f, 0.0f, 1.0f, 1.0f))
            .OnClick([]() {

            })
            .Render();

        UICheckBox()
            .Title("Show debug")
            .Position(vec2_new(10.0f, 100.0f))
            .Render(&showDebug);

        UIGroup(showDebug)
            .Padding(vec2_new(16.0f, 10.0f))
            .Color(0x123456)
            .Render([]() {
                UIText("The showDebug is true")
                    .Render();
            });
    }
};

void main()
{
    return UIApplication()
        .Render(UIMainScreen())
        .Start();
}