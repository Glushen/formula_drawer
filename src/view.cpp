#include "view.h"

void fd::v::View::measure() {
    onMeasure();
}

void fd::v::View::layout() {
    onLayout();
}

void fd::v::View::draw(QPainter& painter) const {
    painter.translate(x, y);
    onDraw(painter);
    painter.translate(-x, -y);
}

bool fd::v::View::isHeightSpecified() {
    return true;
}

qreal fd::v::View::getCyToHeightRatio() {
    return cy / h;
}


static QFont& getFont() {
    static auto font = QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/font.ttf")).at(0));
    font.setPointSizeF(50);
    return font;
}

fd::v::TextView::TextView(const std::string& text):
    text(text.c_str()) { }

void fd::v::TextView::onMeasure() {
    auto fontMetrics = QFontMetricsF(getFont());
    w = fontMetrics.boundingRect(text).width() + 12;
    h = fontMetrics.height();
    cy = h/2;
}

void fd::v::TextView::onLayout() {
    // nothing to do
}

void fd::v::TextView::onDraw(QPainter& painter) const {
    painter.setFont(getFont());
    painter.drawText(QRectF(0, 0, w, h), Qt::AlignHCenter, text);
}


fd::v::BracketView::BracketView(qreal width)
    :width(width) { }

void fd::v::BracketView::onMeasure() {
    w = width;
    cy = h/2;
}

void fd::v::BracketView::onLayout() {
    // nothing to do
}

bool fd::v::BracketView::isHeightSpecified() {
    return false;
}

qreal fd::v::BracketView::getCyToHeightRatio() {
    return 0.5;
}


static void relCubicTo(QPainterPath& path, qreal dx1, qreal dy1, qreal dx2, qreal dy2, qreal dx, qreal dy) {
    auto cur = path.currentPosition();
    path.cubicTo(cur + QPointF(dx1, dy1), cur + QPointF(dx2, dy2), cur + QPointF(dx, dy));
}
static void relLineTo(QPainterPath& path, qreal dx, qreal dy) {
    path.lineTo(path.currentPosition() + QPointF(dx, dy));
}


fd::v::OpeningRoundBracketView::OpeningRoundBracketView():
    BracketView(22) { }

void fd::v::OpeningRoundBracketView::onDraw(QPainter& painter) const {
    QPainterPath path(QPointF(18, 16));
    relCubicTo(path, -5.654, 5.654, -7, 12, -7, 24);
    relLineTo(path, 0, h - 76);
    relCubicTo(path, 0, 12, 1.346, 18.346, 7, 24);
    painter.drawPath(path);
}

fd::v::ClosingRoundBracketView::ClosingRoundBracketView():
    BracketView(22) { }

void fd::v::ClosingRoundBracketView::onDraw(QPainter& painter) const {
    QPainterPath path(QPointF(4, 16));
    relCubicTo(path, 5.654, 5.654, 7, 12, 7, 24);
    relLineTo(path, 0, h - 76);
    relCubicTo(path, 0, 12, -1.346, 18.346, -7, 24);
    painter.drawPath(path);
}

fd::v::OpeningCurlyBracketView::OpeningCurlyBracketView():
    BracketView(38) { }

void fd::v::OpeningCurlyBracketView::onDraw(QPainter& painter) const {
    QPainterPath path(QPointF(31, 15));
    auto verticalElementLength = h / 2 - 36;
    relCubicTo(path, -7.9975, 0.11854, -11, 2.505, -11, 10.5);
    relLineTo(path, 0, verticalElementLength);
    relCubicTo(path, 0, 8.0013, -2.9972, 12, -11, 12);
    relCubicTo(path, 8.0028, 0, 11, 3.9987, 11, 12);
    relLineTo(path, 0, verticalElementLength);
    relCubicTo(path, 0, 7.995, 3.0025, 10.381, 11, 10.5);
    painter.drawPath(path);
}


fd::v::ScaleLayout::ScaleLayout(std::unique_ptr<View> child, qreal factor):
    child(std::move(child)), factor(factor) { }

void fd::v::ScaleLayout::onMeasure() {
    child->measure();
    h = child->h * factor;
    w = child->w * factor;
    cy = child->cy * factor;
}

void fd::v::ScaleLayout::onLayout() {
    child->layout();
    child->x = 0;
    child->y = 0;
}

void fd::v::ScaleLayout::onDraw(QPainter& painter) const {
    painter.scale(factor, factor);
    child->draw(painter);
    painter.scale(1 / factor, 1 / factor);
}


fd::v::SmallLayout::SmallLayout(std::unique_ptr<View> child, fd::v::SmallLayoutType type)
    : child(std::make_unique<ScaleLayout>(std::move(child), 1)), type(type) { }

void fd::v::SmallLayout::onMeasure() {
    static auto isTextScaled = false;
    if (isTextScaled) {
        child->factor = 1;
        child->measure();
    } else {
        isTextScaled = true;
        child->factor = 0.6;
        child->measure();
        isTextScaled = false;
    }
    w = child->w;
    h = child->h;
    cy = type == POWER ? h : 0;
}

void fd::v::SmallLayout::onLayout() {
    child->layout();
    child->x = 0;
    child->y = 0;
}

void fd::v::SmallLayout::onDraw(QPainter& painter) const {
    child->draw(painter);
}


fd::v::HorizontalLayout::HorizontalLayout(std::vector<std::unique_ptr<View>> children):
    children(std::move(children)) { }

void fd::v::HorizontalLayout::onMeasure() {
    w = 0;
    cy = 0;
    qreal hMinusCy = 0;
    for (auto& child : children) {
        if (!child->isHeightSpecified()) {
            continue;
        }
        child->measure();
        w += child->w;
        cy = std::max(cy, child->cy);
        hMinusCy = std::max(hMinusCy, child->h - child->cy);
    }
    h = hMinusCy + cy;
    for (auto& child : children) {
        if (child->isHeightSpecified()) {
            continue;
        }
        auto cyToHeight = child->getCyToHeightRatio();
        child->h = 2 * std::min(cyToHeight, 1 - cyToHeight) * h;
        child->measure();
        w += child->w;
    }
}

void fd::v::HorizontalLayout::onLayout() {
    qreal currentX = 0;
    for (auto& child : children) {
        child->layout();
        child->x = currentX;
        currentX += child->w;
        child->y = cy - child->cy;
    }
}

void fd::v::HorizontalLayout::onDraw(QPainter& painter) const {
    for (auto& child : children) {
        child->draw(painter);
    }
}


fd::v::FractionLayout::FractionLayout(std::unique_ptr<View> num, std::unique_ptr<View> den):
    num(std::move(num)), den(std::move(den)) { }

void fd::v::FractionLayout::onMeasure() {
    num->measure();
    den->measure();
    w = std::max(num->w, den->w) + 6;
    h = num->h + den->h;
    cy = num->h;
}

void fd::v::FractionLayout::onLayout() {
    num->layout();
    den->layout();
    num->x = (w - num->w) / 2;
    num->y = 0;
    den->x = (w - den->w) / 2;
    den->y = num->h;
}

void fd::v::FractionLayout::onDraw(QPainter& painter) const {
    num->draw(painter);
    painter.drawLine(QLineF(4, cy, w-4, cy));
    den->draw(painter);
}


fd::v::TripleVerticalLayout::TripleVerticalLayout(std::unique_ptr<View> top, std::unique_ptr<View> center, std::unique_ptr<View> bottom):
    top(std::move(top)), center(std::move(center)), bottom(std::move(bottom)) { }

void fd::v::TripleVerticalLayout::onMeasure() {
    top->measure();
    center->measure();
    bottom->measure();
    w = std::max({top->w, center->w, bottom->w});
    h = top->h + center->h + bottom->h;
    cy = top->h + center->cy;
}

void fd::v::TripleVerticalLayout::onLayout() {
    top->layout();
    center->layout();
    bottom->layout();
    top->x = (w - top->w);
    top->y = 0;
    center->x = (w - center->w);
    center->y = top->h;
    bottom->x = (w - bottom->w);
    bottom->y = top->h + center->h;
}

void fd::v::TripleVerticalLayout::onDraw(QPainter& painter) const {
    top->draw(painter);
    center->draw(painter);
    bottom->draw(painter);
}


fd::v::GridLayout::GridLayout(std::vector<std::vector<std::unique_ptr<View>>> rows): rows(std::move(rows)) { }

void fd::v::GridLayout::onMeasure() {
    w = 0;
    h = 0;
    cy = 0;
    if (rows.empty()) {
        return;
    }

    rowHeights.clear();
    rowCys.clear();
    size_t columnsCount = 0;

    for (const auto& row : rows) {
        qreal currentCy = 0, currentHMinusCy = 0;
        for (auto& view : row) {
            view->measure();
            currentCy = std::max(currentCy, view->cy);
            currentHMinusCy = std::max(currentHMinusCy, view->h - view->cy);
        }
        rowCys.push_back(currentCy + 12);
        auto height = currentHMinusCy + currentCy + 24;
        rowHeights.push_back(height);
        h += height;
        columnsCount = std::max(columnsCount, row.size());
    }

    columnWidths.clear();

    for (int i = 0; i < columnsCount; i++) {
        qreal currentW = 0;
        for (const auto& row : rows) {
            if (i < row.size()) {
                currentW = std::max(currentW, row[i]->w);
            }
        }
        currentW += 24;
        columnWidths.push_back(currentW);
        w += currentW;
    }

    cy = h / 2;
}

void fd::v::GridLayout::onLayout() {
    qreal currentY = 0;
    for (int i = 0; i < rows.size(); i++) {
        qreal currentX = 0;
        for (int j = 0; j < rows[i].size(); j++) {
            rows[i][j]->layout();
            rows[i][j]->x = currentX + (columnWidths[j] - rows[i][j]->w) / 2;
            currentX += columnWidths[j];
            rows[i][j]->y = currentY + rowCys[i] - rows[i][j]->cy;
        }
        currentY += rowHeights[i];
    }
}

void fd::v::GridLayout::onDraw(QPainter& painter) const {
    for (const auto& row : rows) {
        for (auto& view : row) {
            view->draw(painter);
        }
    }
}
