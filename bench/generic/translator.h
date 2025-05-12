
#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <variable.h>

#include <QByteArray>

#include <QHash>


class Translator
{
public:
    Translator();
    Translator(const QString &data);
    typedef enum {KEY,VALUE} Side;
    ~Translator() {}
    /* populates table of translation data format "0:<value1>;1:<value2>;...*/
    void populateTable(const QString &script);
    /* gets value after translation */
    const Variable get(Side side, const Variable &item);

    const QStringList   items(Side side) ;



private:

    QHash<QString , QString> m_key_value;
    QString m_formula;
    int m_err;
    char* mm_err_pos;
    int m_paren_count;

    enum EXPR_EVAL_ERR {
        TR_NO_ERROR = 0,
        TR_PARENTHESIS = 1,
        TR_WRONG_CHAR = 2,
        TR_DIVIDE_BY_ZERO = 3
    };
    // Parse a number or an expression in parenthesis
    double ParseAtom(char*& expr)
    {
        // Skip spaces
        while(*expr == ' ')
            expr++;

        // Handle the sign before parenthesis (or before number)
        bool negative = false;
        if(*expr == '-') {
            negative = true;
            expr++;
        }
        if(*expr == '+') {
            expr++;
        }

        // Check if there is parenthesis
        if(*expr == '(') {
            expr++;
            m_paren_count++;
            double res = ParseSummands(expr);
            if(*expr != ')') {
                // Unmatched opening parenthesis
                m_err = TR_PARENTHESIS;
                mm_err_pos = expr;
                return 0;
            }
            expr++;
            m_paren_count--;
            return negative ? -res : res;
        }

        // It should be a number; convert it to double
        char* end_ptr;
        double res = strtod(expr, &end_ptr);
        if(end_ptr == expr) {
            // Report error
            m_err = TR_WRONG_CHAR;
            mm_err_pos = expr;
            return 0;
        }
        // Advance the pointer and return the result
        expr = end_ptr;
        return negative ? -res : res;
    }


    // Parse addition and subtraction
    double ParseSummands(char*& expr)
    {
        double num1 = ParseFactors(expr);
        for(;;) {
            // Skip spaces
            while(*expr == ' ')
                expr++;
            char op = *expr;
            if(op != '-' && op != '+')
                return num1;
            expr++;
            double num2 = ParseFactors(expr);
            if(op == '-')
                num1 -= num2;
            else
                num1 += num2;
        }
    }
    // Parse multiplication and division
    double ParseFactors(char*& expr)
    {
        double num1 = ParseAtom(expr);
        for(;;) {
            // Skip spaces
            while(*expr == ' ')
                expr++;
            // Save the operation and position
            char op = *expr;
            char* pos = expr;
            if(op != '/' && op != '*')
                return num1;
            expr++;
            double num2 = ParseAtom(expr);
            // Perform the saved operation
            if(op == '/') {
                // Handle division by zero
                if(num2 == 0) {
                    m_err = TR_DIVIDE_BY_ZERO;
                    mm_err_pos = pos;
                    return 0;
                }
                num1 /= num2;
            }
            else
                num1 *= num2;
        }
    }

    double Eval(char* expr)
    {
        m_paren_count = 0;
        m_err = TR_NO_ERROR;
        double res = ParseSummands(expr);
        // Now, expr should point to '\0', and m_paren_count should be zero
        if(m_paren_count != 0 || *expr == ')')
        {
            m_err = TR_PARENTHESIS;
            mm_err_pos = expr;
            return 0;
        }
        if(*expr != '\0')
        {
            m_err = TR_WRONG_CHAR;
            mm_err_pos = expr;
            return 0;
        }
        return res;
    };

    int GetErr()
    {
        return m_err;
    }
    char* GetErrPos()
    {
        return mm_err_pos;
    }


};
#endif
