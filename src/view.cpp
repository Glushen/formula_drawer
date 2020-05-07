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

fd::v::TextView::TextView(const std::string& text): text(text.c_str()) { }

void fd::v::TextView::onMeasure() {
    static auto fontMetrics = QFontMetricsF(QFont());
    w = fontMetrics.horizontalAdvance(text);
    h = fontMetrics.height();
    cy = h/2;
}

void fd::v::TextView::onLayout() {
    // nothing to do
}

void fd::v::TextView::onDraw(QPainter& painter) const {
    painter.drawText(0, 0, text);
}


fd::v::TranslateLayout::TranslateLayout(std::unique_ptr<View> child, qreal translationY):
    child(std::move(child)), translationY(translationY) { }

void fd::v::TranslateLayout::onMeasure() {
    child->measure();
    w = child->w;
    targetCy = child->cy - translationY;
    if (targetCy < 0) {
        h = child->h + std::abs(targetCy);
        cy = 0;
    } else if (targetCy > child->h) {
        h = targetCy;
        cy = targetCy;
    } else {
        h = child->h;
        cy = targetCy;
    }
}

void fd::v::TranslateLayout::onLayout() {
    child->x = 0;
    child->y = targetCy < 0 ? std::abs(targetCy) : 0;
}

void fd::v::TranslateLayout::onDraw(QPainter& painter) const {
    child->draw(painter);
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
    child->x = 0;
    child->y = 0;
}

void fd::v::ScaleLayout::onDraw(QPainter& painter) const {
    painter.scale(factor, factor);
    child->draw(painter);
    painter.scale(1 / factor, 1 / factor);
}


fd::v::AutoscaleLayout::AutoscaleLayout(std::unique_ptr<View> child):
    child(std::move(child)) { }

void fd::v::AutoscaleLayout::onMeasure() {
    if (!isChildMeasured) {
        child->measure();
        isChildMeasured = true;
    }
    auto scaleFactor = h / child->h;
    w = child->w * scaleFactor;
    cy = child->cy * scaleFactor;
}

void fd::v::AutoscaleLayout::onLayout() {
    child->x = 0;
    child->y = 0;
}

void fd::v::AutoscaleLayout::onDraw(QPainter& painter) const {
    auto scaleFactor = h / child->h;
    painter.scale(scaleFactor, scaleFactor);
    child->draw(painter);
    painter.scale(1 / scaleFactor, 1 / scaleFactor);
}

bool fd::v::AutoscaleLayout::isHeightSpecified() {
    return false;
}

qreal fd::v::AutoscaleLayout::getCyToHeightRatio() {
    if (!isChildMeasured) {
        child->measure();
        isChildMeasured = true;
    }
    return child->cy / child->h;
}


fd::v::HorizontalLayout::HorizontalLayout(std::vector<std::unique_ptr<View>> children):
    children(std::move(children)) { }

void fd::v::HorizontalLayout::onMeasure() {
    w = 0;
    cy = 0;
    qreal hMinusCy = 0;
    for (auto& child : children) {
        if (!isHeightSpecified()) {
            continue;
        }
        child->measure();
        w += child->w;
        cy = std::max(cy, child->cy);
        hMinusCy = std::max(hMinusCy, child->h - child->cy);
    }
    h = hMinusCy + cy;
    for (auto& child : children) {
        if (isHeightSpecified()) {
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
    w = std::max(num->w, den->w);
    h = num->h + den->h;
    cy = num->h;
}

void fd::v::FractionLayout::onLayout() {
    num->x = (w - num->w) / 2;
    num->y = 0;
    den->x = (w - den->w) / 2;
    den->y = num->h;
}

void fd::v::FractionLayout::onDraw(QPainter& painter) const {
    num->draw(painter);
    painter.drawLine(0, cy, w, cy);
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
        rowCys.push_back(currentCy);
        rowHeights.push_back(currentHMinusCy + currentCy);
        h += currentHMinusCy + currentCy;
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
