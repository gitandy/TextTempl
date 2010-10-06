#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    textFormat.setFontItalic(true);
    textFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\[\\[[a-zA-Z0-9_\\-]+@.*\\]\\]");
    rule.pattern.setMinimal(true);
    rule.format = textFormat;
    highlightingRules.append(rule);

    keywordFormat.setFontItalic(true);
    keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\[\\[" + QString(QT_TRANSLATE_NOOP("MainApp", "Name")) + "@.*\\]\\]");
    rule.pattern.setMinimal(true);
    rule.format = keywordFormat;
    highlightingRules.append(rule);

    commentFormat.setFontItalic(true);
    commentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("##.*\\]\\]");
    rule.pattern.setMinimal(true);
    rule.format = commentFormat;
    highlightingRules.append(rule);

    sepFormat.setFontWeight(QFont::Bold);
    sepFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\[\\[");
    rule.format = sepFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("\\]\\]");
    rule.format = sepFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("@");
    rule.format = sepFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("::");
    rule.format = sepFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
