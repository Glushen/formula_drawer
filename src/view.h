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

        explicit TextView(const std::string& text);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;
    };

    class TranslateLayout : public View {
    public:
        std::unique_ptr<View> child;
        qreal translationY;

        TranslateLayout(std::unique_ptr<View> child, qreal translationY);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;

    private:
        qreal targetCy = 0;
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

    class AutoscaleLayout : public View {
    public:
        std::unique_ptr<View> child;

        explicit AutoscaleLayout(std::unique_ptr<View> child);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter &painter) const override;

        bool isHeightSpecified() override;

        qreal getCyToHeightRatio() override;

    private:
        bool isChildMeasured = false;
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

        explicit GridLayout(std::vector<std::vector<std::unique_ptr<View>>> rows);

        void onMeasure() override;
        void onLayout() override;
        void onDraw(QPainter& painter) const override;

    private:
        std::vector<qreal> rowHeights = {};
        std::vector<qreal> rowCys = {};
        std::vector<qreal> columnWidths = {};
    };
}
