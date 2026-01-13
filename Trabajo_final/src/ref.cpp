/**
 * @file ref.cpp
 * @brief Implementación del generador de señales de referencia.
 */

#include "ref.h"

#include <stdexcept>

namespace RefSignal {

    /// Constante 2π para cálculos trigonométricos
    static constexpr double TWO_PI = 6.28318530717958647692;

    /*========================================================================*/
    /*                     CLASE BASE: Signal                                 */
    /*========================================================================*/

    /**
     * @brief Añade un par (time, value) al buffer circular.
     *
     * Si el buffer está lleno, elimina el elemento más antiguo.
     */
    void Signal::addToBuffer(double time, double value) {
        if (time_buffer_.size() >= buffer_size_) {
            time_buffer_.pop_front();
            value_buffer_.pop_front();
        }
        time_buffer_.push_back(time);
        value_buffer_.push_back(value);
    }

    Signal::Signal(double Ts, double offset, std::size_t buffer_size)
        : Ts_(Ts), offset_(offset), t_(0.0), buffer_size_(buffer_size) {
        if (Ts_ <= 0.0) {
            throw std::invalid_argument("Signal: Ts debe ser > 0");
        }
        if (buffer_size_ == 0) {
            throw std::invalid_argument("Signal: buffer_size debe ser >= 1");
        }
    }

    double Signal::compute() const {
        return computeAt(t_);
    }

    double Signal::compute(std::size_t k) const {
        double time = static_cast<double>(k) * Ts_;
        return computeAt(time);
    }

    double Signal::next() {
        double value = compute();
        addToBuffer(t_, value);
        t_ += Ts_;
        return value;
    }

    void Signal::reset() {
        t_ = 0.0;
        time_buffer_.clear();
        value_buffer_.clear();
    }

    /*--- Getters / Setters ---*/

    double& Signal::T() {
        return Ts_;
    }
    const double& Signal::T() const {
        return Ts_;
    }

    double& Signal::offset() {
        return offset_;
    }
    const double& Signal::offset() const {
        return offset_;
    }

    double& Signal::t() {
        return t_;
    }
    const double& Signal::t() const {
        return t_;
    }

    std::size_t& Signal::bufferSize() {
        return buffer_size_;
    }
    const std::size_t& Signal::bufferSize() const {
        return buffer_size_;
    }

    const std::deque<double>& Signal::timeBuffer() const {
        return time_buffer_;
    }
    const std::deque<double>& Signal::valueBuffer() const {
        return value_buffer_;
    }

    /**
     * @brief Operador de salida para serializar buffers en formato CSV.
     *
     * Formato: time,value por línea.
     */
    std::ostream& operator<<(std::ostream& os, const Signal& s) {
        for (std::size_t i = 0; i < s.time_buffer_.size(); ++i) {
            os << s.time_buffer_[i] << "," << s.value_buffer_[i] << "\n";
        }
        return os;
    }

    /*========================================================================*/
    /*                          SEÑAL ESCALÓN                                 */
    /*========================================================================*/

    StepSignal::StepSignal(double Ts, double amplitude, double step_time,
                           double offset, std::size_t buffer_size)
        : Signal(Ts, offset, buffer_size),
          amplitude_(amplitude),
          step_time_(step_time) {}

    double StepSignal::computeAt(double time) const {
        return (time >= step_time_) ? amplitude_ + offset_ : offset_;
    }

    double& StepSignal::amplitude() {
        return amplitude_;
    }
    const double& StepSignal::amplitude() const {
        return amplitude_;
    }

    double& StepSignal::stepTime() {
        return step_time_;
    }
    const double& StepSignal::stepTime() const {
        return step_time_;
    }

    /*========================================================================*/
    /*                          SEÑAL RAMPA                                   */
    /*========================================================================*/

    RampSignal::RampSignal(double Ts, double slope, double start_time,
                           double offset, std::size_t buffer_size)
        : Signal(Ts, offset, buffer_size),
          slope_(slope),
          start_time_(start_time) {}

    double RampSignal::computeAt(double time) const {
        if (time < start_time_) {
            return offset_;
        } else {
            return slope_ * (time - start_time_) + offset_;
        }
    }

    double& RampSignal::slope() {
        return slope_;
    }
    const double& RampSignal::slope() const {
        return slope_;
    }

    double& RampSignal::startTime() {
        return start_time_;
    }
    const double& RampSignal::startTime() const {
        return start_time_;
    }

    /*========================================================================*/
    /*                        SEÑAL SINUSOIDAL                                */
    /*========================================================================*/

    SineSignal::SineSignal(double Ts, double amplitude, double freq,
                           double phase, double offset,
                           std::size_t buffer_size)
        : Signal(Ts, offset, buffer_size),
          amplitude_(amplitude),
          freq_(freq),
          phase_(phase) {}

    double SineSignal::computeAt(double time) const {
        return amplitude_ * std::sin(TWO_PI * freq_ * time + phase_) + offset_;
    }

    double& SineSignal::amplitude() {
        return amplitude_;
    }
    const double& SineSignal::amplitude() const {
        return amplitude_;
    }

    double& SineSignal::frequency() {
        return freq_;
    }
    const double& SineSignal::frequency() const {
        return freq_;
    }

    double& SineSignal::phase() {
        return phase_;
    }
    const double& SineSignal::phase() const {
        return phase_;
    }

} // namespace RefSignal
