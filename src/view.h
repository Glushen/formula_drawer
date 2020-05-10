#pragma once

#include <memory>
#include <vector>
#include <QtWidgets>

namespace fd::v {
    class View {
    public:
        qreal x = 0, y = 0, w = 0, h = 0, cy = 0;

        void measure();
        void layout();
        void draw(QPainter& painter) const;

        virtual void onMeasure() = 0;
        virtual void onLayout() = 0;
        virtual void onDraw(QPainter& painter) const = 0;

        virtual bool isHeightSpecified();
        virtual qreal getCyToHeightRatio();

        virtual ~View() = default;
    };

    class TextView : public View {
    public:
        QString text;
        bool variadicSymbol = false;

        explicit TextView(const std::string& text, bool variadicSymbol = false);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;
    };

    class BracketView : public View {
    public:
        qreal width;

        explicit BracketView(qreal width);

        void onMeasure() override;
        void onLayout() override;
        bool isHeightSpecified() override;
        qreal getCyToHeightRatio() override;
    };

    class OpeningRoundBracketView : public BracketView {
    public:
        OpeningRoundBracketView();

        void onDraw(QPainter& painter) const override;
    };

    class ClosingRoundBracketView : public BracketView {
    public:
        ClosingRoundBracketView();

        void onDraw(QPainter& painter) const override;
    };

    class OpeningCurlyBracketView : public BracketView {
    public:
        OpeningCurlyBracketView();

        void onDraw(QPainter& painter) const override;
    };

    class ScaleLayout : public View {
    public:
        std::unique_ptr<View> child;
        qreal factor;

        ScaleLayout(std::unique_ptr<View> child, qreal factor);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter &painter) const override;
    };

    enum SmallLayoutType {
        POWER, INDEX, NONE
    };

    class SmallLayout : public View {
    public:
        std::unique_ptr<ScaleLayout> child;
        SmallLayoutType type;

        SmallLayout(std::unique_ptr<View> child, SmallLayoutType type);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;
    };

    class HorizontalLayout : public View {
    public:
        std::vector<std::unique_ptr<View>> children;

        explicit HorizontalLayout(std::vector<std::unique_ptr<View>> children);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;
    };

    class FractionLayout : public View {
    public:
        std::unique_ptr<View> num;
        std::unique_ptr<View> den;

        FractionLayout(std::unique_ptr<View> num, std::unique_ptr<View> den);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;
    };

    class TripleVerticalLayout : public View {
    public:
        std::unique_ptr<View> top;
        std::unique_ptr<View> center;
        std::unique_ptr<View> bottom;

        TripleVerticalLayout(std::unique_ptr<View> top, std::unique_ptr<View> center, std::unique_ptr<View> bottom);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;
    };

    class GridLayout : public View {
    public:
        std::vector<std::vector<std::unique_ptr<View>>> rows;
        bool alignCenter;

        GridLayout(std::vector<std::vector<std::unique_ptr<View>>> rows, bool alignCenter);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;

    private:
        std::vector<qreal> rowHeights = {};
        std::vector<qreal> rowCys = {};
        std::vector<qreal> columnWidths = {};
    };
}
