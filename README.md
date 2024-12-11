# QThrobber
Customizable throbber widget for Qt that uses QPainter rather than gifs to provide a clean and undistorted indicator. The throbber rotates, shrinks and grows as a helpful way to indiciate a state of busyness, loading or processing.

## Example
``` C++
    QThrobber *throbber = new QThrobber(ui->widget);
    throbber->setFixedSize(150,150);
    throbber->setText("Loading");
    throbber->setSpeed(QThrobber::Normal);
    throbber->setIcon(QIcon(":/icons/icons8-temperature.svg"));
    ui->widget->layout()->addWidget(throbber);
```

If desired, you can pass a configured QPen object to the `setPen(...)` method. Setting the pen will override the size and color set with `setThrobberWidth(...)` and `setThrobberColor(...)` methods.

This is the preferred way to use color gradiants and dash styles
``` C++
    QPen pen;
    pen.setWidth(4);
    pen.setColor("#F1F44E");
    pen.setCapStyle(Qt::RoundCap);
```

The icon passed to QThrobber can be set to animate it's scale and/or opacity with the `setIconScaleAnimationEnabled(...)` and `setIconOpacityAnimationEnabled(...)`. By default, only the icon opacity animation is enabled

Included example uses an icon from Icons8.com
